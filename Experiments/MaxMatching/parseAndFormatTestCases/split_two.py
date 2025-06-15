"""
Reformat CSES 'Split into Two Paths' for bipartite problem test cases.
Download from: https://cses.fi/problemset/tests/3358/ (login required).

Original input:
 N M
 u_i v_i (1-indexed)

Reformatted:
 N N M
 u_i-1 v_i-1
"""

from pathlib import Path
import zipfile

ZIP_FILE = Path(__file__).parents[2] / 'related_problems' / '11_split_two.zip'
TMP_DIR = Path(__file__).parent / 'tmp'
OUT_DIR = Path(__file__).parent.parent / 'testCases'

TMP_DIR.mkdir(exist_ok=True)
OUT_DIR.mkdir(exist_ok=True)

with zipfile.ZipFile(ZIP_FILE, 'r') as z:
    z.extractall(TMP_DIR)

for idx, in_path in enumerate(TMP_DIR.rglob('*.in')):
    lines = in_path.read_text().splitlines()
    N, M = map(int, lines[0].split())
    header = f"{N} {N} {M}"
    edges = [f"{int(u)-1} {int(v)-1}" for u, v in (line.split() for line in lines[1:])]
    out_file = OUT_DIR / f"split_two-{idx}.in"
    out_file.write_text('\n'.join([header] + edges) + '\n')

# cleanup
for f in TMP_DIR.iterdir(): f.unlink()
TMP_DIR.rmdir()