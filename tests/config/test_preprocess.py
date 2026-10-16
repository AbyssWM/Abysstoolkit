#!/usr/bin/env python3
"""Dependency-free CLI and CMake regressions. Artifacts stay under _builds."""

import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest


REPO = Path(__file__).resolve().parents[2]
TOOL = REPO / 'tools/abyss-preprocess/abyss-preprocess'
WORK = Path(os.environ.get('ABYSS_TEST_WORK_DIR', REPO.parent / '_builds/abysstoolkit-config-tests'))


class PreprocessTests(unittest.TestCase):
    def setUp(self):
        WORK.mkdir(parents=True, exist_ok=True)
        self.root = Path(tempfile.mkdtemp(prefix='case-', dir=WORK))
        self.config = self.root / 'config'
        self.config.mkdir()
        self.output = self.root / 'processed'
        self.header = self.root / 'generated/configs.hpp'

    def run_tool(self, *, ok=True, app='paper', env=None):
        result = subprocess.run([sys.executable, '-B', str(TOOL), '--app', app,
                                 '--config-dir', str(self.config), '--output-dir', str(self.output),
                                 '--generate-header', str(self.header)],
                                capture_output=True, text=True, env=env)
        self.assertEqual(result.returncode == 0, ok, result.stdout + result.stderr)
        return result

    def put(self, name, body, app='paper'):
        (self.config / name).write_text(f'#include.abyss <{app}>\n{body}\n', encoding='utf-8')

    def compile(self, source):
        compiler = shutil.which('c++')
        if not compiler:
            self.skipTest('c++ compiler unavailable')
        result = subprocess.run([compiler, '-std=c++17', '-pedantic-errors', '-Werror',
                                 '-fsyntax-only', '-x', 'c++', '-'],
                                input=f'#include "{self.header}"\n{source}\n', text=True,
                                capture_output=True)
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_types_comments_and_expanded_strings_compile(self):
        self.put('my-config.hpp', '''wallpaper = "$ABYSS_TEST_VALUE" // comment
volume = 40 // int
opacity = 0.85 // float
scale = 1e-2
audio = true
url = "https://example.test/a//b"
low = -2147483648
high = 2147483647
escaped = "a\\n\\t\\\"b\\\\c\\u0000z"''')
        environment = dict(os.environ, ABYSS_TEST_VALUE='path\n"; #error injected\n\\end')
        self.run_tool(env=environment)
        self.compile('''static_assert(abyss::paper::NUM_CONFIGS == 1);
static_assert(abyss::paper::my_config::volume == 40);
static_assert(abyss::paper::my_config::audio);
static_assert(abyss::paper::my_config::low == -2147483647 - 1);
static_assert(abyss::paper::my_config::escaped[1] == '\\n');''')

    def test_invalid_input_fails_without_aggregate(self):
        for body in ('wallpaper = unquoted', 'class = 1', 'x; int injected = 1',
                     'a = 1\na = 2', 'large = 2147483648', 'bad = 1.0e100',
                     'bad = 1.0e-100', 'bad = 1e-9999', 'bad = "a" "b"', '#include "helpers.hpp"',
                     'bad = "\\ud800"'):
            with self.subTest(body=body):
                self.put('bad.hpp', body)
                result = self.run_tool(ok=False)
                self.assertIn('ERROR', result.stderr)
                self.assertFalse(self.header.exists())

    def test_invalid_app_and_directive(self):
        self.run_tool(ok=False, app='paper::injected')
        (self.config / 'bad.hpp').write_text('#include.abyss <paper> garbage\na = 1\n')
        self.run_tool(ok=False)

    def test_filename_data_cannot_inject_cpp(self):
        for name in ('9 name.hpp', 'class.hpp', 'quote"name.hpp', 'ümlaut.hpp', 'NUM_CONFIGS.hpp'):
            self.put(name, 'value = 1')
        self.run_tool()
        self.compile('static_assert(abyss::paper::NUM_CONFIGS == 5);')

    def test_namespace_collision_is_reported(self):
        self.put('a-b.hpp', 'value = 1')
        self.put('a.b.hpp', 'value = 2')
        self.assertIn('collision', self.run_tool(ok=False).stderr)

    def test_empty_and_foreign_headers(self):
        self.put('lock.hpp', 'not valid paper data', app='lock')
        (self.config / 'ordinary.hpp').write_text('#pragma once\n')
        (self.config / 'empty.hpp').write_text('')
        self.run_tool()
        self.compile('static_assert(abyss::paper::NUM_CONFIGS == 0);')

    def test_stable_outputs_and_deletion(self):
        self.put('one.hpp', 'value = 1')
        self.run_tool()
        before = {path: path.stat().st_mtime_ns for path in (self.header, *self.output.iterdir())}
        self.run_tool()
        self.assertEqual(before, {path: path.stat().st_mtime_ns for path in before})
        # Move the config out of the scanned directory to exercise removal.
        (self.config / 'one.hpp').rename(self.root / 'one.hpp')
        self.run_tool()
        self.assertEqual(json.loads((self.output / 'manifest.json').read_text())['configs'], [])
        self.compile('static_assert(abyss::paper::NUM_CONFIGS == 0);')

    def test_io_errors_fail_cleanly(self):
        self.output.write_text('not a directory')
        result = self.run_tool(ok=False)
        self.assertIn('ERROR:', result.stderr)
        self.assertNotIn('Traceback', result.stderr)

    @unittest.skipUnless(shutil.which('cmake') and shutil.which('c++'), 'cmake/c++ unavailable')
    def test_cmake_rebuild_tracks_changes_and_environment(self):
        source = self.root / 'source'
        source.mkdir()
        (source / 'CMakeLists.txt').write_text(f'''cmake_minimum_required(VERSION 3.19)
project(config_smoke LANGUAGES CXX)
include("{REPO}/cmake/AbyssConfig.cmake")
add_executable(check main.cpp)
abyss_add_config_support(check paper)
''')
        main = source / 'main.cpp'
        main.write_text('#include "checkConfigs.hpp"\nint main() {return abyss::paper::NUM_CONFIGS;}\n')
        home = self.root / 'home'
        config = home / '.config/abyss'
        config.mkdir(parents=True)
        environment = dict(os.environ, HOME=str(home), XDG_CONFIG_HOME='', ABYSS_TEST_VALUE='first')
        build = self.root / 'build'

        def run(command):
            result = subprocess.run(command, capture_output=True, text=True, env=environment)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

        run(['cmake', '-S', str(source), '-B', str(build)])
        command = ['cmake', '--build', str(build), '--parallel', '2']
        run(command)
        generated = build / 'generated/checkConfigs.hpp'
        self.assertIn('NUM_CONFIGS = 0', generated.read_text())
        config_file = config / 'one.hpp'
        config_file.write_text('#include.abyss <paper>\nvalue = "$ABYSS_TEST_VALUE"\n')
        run(command)
        self.assertIn('NUM_CONFIGS = 1', generated.read_text())
        processed = build / 'abyss-config/check/one.processed.hpp'
        self.assertIn('"first"', processed.read_text())
        environment['ABYSS_TEST_VALUE'] = 'second'
        run(command)
        self.assertIn('"second"', processed.read_text())
        binary_mtime = (build / 'check').stat().st_mtime_ns
        run(command)
        self.assertEqual((build / 'check').stat().st_mtime_ns, binary_mtime)
        config_file.rename(self.root / 'removed.hpp')
        run(command)
        self.assertIn('NUM_CONFIGS = 0', generated.read_text())
        # Test modification of an existing file and propagation of parse failure.
        config_file.write_text('#include.abyss <paper>\nvalue = 1\n')
        run(command)
        config_file.write_text('#include.abyss <paper>\nvalue = invalid\n')
        failed = subprocess.run(command, capture_output=True, text=True, env=environment)
        self.assertNotEqual(failed.returncode, 0)
        config_file.write_text('#include.abyss <paper>\nvalue = 2\n')
        run(command)
        (build / 'abyss-config').rename(self.root / 'moved-cache')
        generated.rename(self.root / 'moved-aggregate.hpp')
        run(command)
        self.assertIn('value = 2', processed.read_text())
        # XDG paths and an explicit packaging/test override are configure inputs.
        xdg = self.root / 'xdg'
        (xdg / 'abyss').mkdir(parents=True)
        (xdg / 'abyss/one.hpp').write_text('#include.abyss <paper>\nvalue = 3\n')
        environment['XDG_CONFIG_HOME'] = str(xdg)
        configure = ['cmake', '-S', str(source), '-B', str(build)]
        run(configure)
        run(command)
        self.assertIn('value = 3', processed.read_text())
        environment['XDG_CONFIG_HOME'] = 'relative/path/is/ignored'
        run(configure)
        run(command)
        self.assertIn('value = 2', processed.read_text())
        override = self.root / 'override'
        override.mkdir()
        (override / 'one.hpp').write_text('#include.abyss <paper>\nvalue = 4\n')
        run([*configure, f'-DABYSS_CONFIG_DIR={override}'])
        run(command)
        self.assertIn('value = 4', processed.read_text())


if __name__ == '__main__':
    unittest.main(verbosity=2)
