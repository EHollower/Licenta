"""
Reformat CSES “Police Chase” test cases (1-indexed) into 0-indexed format.
Download from: https://cses.fi/problemset/tests/1695/ (login required)

Original input:
V E
u_i v_i 1

Reformatted:
V E s t
u_i v_i 1
where s = 0, t = V - 1.
"""

from pathlib import Path
import zipfile

# paths
ZIP_FILE = Path(__file__).parents[2] / 'related_problems' / '2_police_chase.zip'
TMP_DIR  = Path(__file__).parent / 'tmp'
OUT_DIR  = Path(__file__).parent.parent / 'testCases'

TMP_DIR.mkdir(exist_ok=True)
OUT_DIR.mkdir(exist_ok=True)

# extract all .in files
with zipfile.ZipFile(ZIP_FILE, 'r') as z:
    z.extractall(TMP_DIR)

# reformat and write
for idx, in_path in enumerate(TMP_DIR.rglob('*.in')):
    lines = in_path.read_text().splitlines()
    V, E = map(int, lines[0].split())
    header = f"{V} {E} 0 {V-1}"
    edges = [f"{int(u)-1} {int(v)-1} 1"
             for u, v, *_ in (line.split() for line in lines[1:])]
    out_file = OUT_DIR / f"policechase-{idx}.in"
    out_file.write_text('\n'.join([header] + edges) + '\n')

for f in TMP_DIR.iterdir():
    f.unlink()

TMP_DIR.rmdir()
