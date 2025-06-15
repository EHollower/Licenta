"""
Reformat CSES “School Dance” matching cases into L R M with zero-based vertices.
Download from: https://cses.fi/problemset/tests/1696/ (login required).

Original input:
 L R E
 u_i v_i (1-indexed)

Output:
 L R M
 u v (0-indexed)
"""

from pathlib import Path
import zipfile

ZIP_FILE = Path(__file__).parents[2] / 'related_problems' / '3_school_dance.zip'
TMP_DIR = Path(__file__).parent / 'tmp'
OUT_DIR = Path(__file__).parent.parent / 'testCases'

TMP_DIR.mkdir(exist_ok=True)
OUT_DIR.mkdir(exist_ok=True)

with zipfile.ZipFile(ZIP_FILE, 'r') as z:
    z.extractall(TMP_DIR)

for idx, in_path in enumerate(TMP_DIR.rglob('*.in')):
    lines = in_path.read_text().splitlines()
    L, R, E = map(int, lines[0].split())
    header = f"{L} {R} {E}"
    edges = [f"{int(u)-1} {int(v)-1}" for u,v in (line.split() for line in lines[1:])]
    out_file = OUT_DIR / f"schooldance-{idx}.in"
    out_file.write_text('\n'.join([header] + edges) + '\n')

# cleanup
for f in TMP_DIR.iterdir(): f.unlink()
TMP_DIR.rmdir()
