"""
Reformat CSES “Distinct Routes” test cases (1-indexed) into 0-indexed format.
Download the test cases from: https://cses.fi/problemset/tests/1711/ (login required)

Original input:
V E
u_i v_i 1

Reformatted:
V E s t
u_i v_i 1
where s=0, t=V-1.
"""

from pathlib import Path
import zipfile

# paths
ZIP_FILE = Path(__file__).parents[2] / 'related_problems' / '4_distinct_routes.zip'
TMP_DIR  = Path(__file__).parent / 'tmp'
OUT_DIR  = Path(__file__).parent.parent / 'testCases'

TMP_DIR.mkdir(exist_ok=True)
OUT_DIR.mkdir(exist_ok=True)

with zipfile.ZipFile(ZIP_FILE) as z:
    z.extractall(TMP_DIR)

for idx, in_file in enumerate(TMP_DIR.glob('*.in')):
    lines = in_file.read_text().splitlines()
    V, E = map(int, lines[0].split())
    header = f"{V} {E} 0 {V-1}"
    edges  = [
        f"{int(u)-1} {int(v)-1} 1"
        for u, v, *_ in (line.split() for line in lines[1:])
    ]
    out_file = OUT_DIR / f"distinctroutes-{idx}.in"
    out_file.write_text('\n'.join([header] + edges) + '\n')

# clean up
for f in TMP_DIR.iterdir(): f.unlink()
TMP_DIR.rmdir()