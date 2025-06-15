"""
Reformat CSES “School Dance” test cases (1-indexed) into maxflow input (0-indexed).
Download from: https://cses.fi/problemset/tests/1696/ (login required)

Original input:
L R E
u_i v_i  (1 ≤ u ≤ L, 1 ≤ v ≤ R)

Reformatted:
N M s t
edges:
s → each left node (cap=1)
left u_i → right (v_i+L) (cap=1)
each right node → t (cap=1)
where N=L+R+2, M=L+E+R, s=L+R, t=L+R+1
"""

from pathlib import Path
import zipfile

ZIP_FILE = Path(__file__).parents[2] / 'related_problems' / '3_school_dance.zip'
TMP_DIR  = Path(__file__).parent / 'tmp'
OUT_DIR  = Path(__file__).parent.parent / 'testCases'

TMP_DIR.mkdir(exist_ok=True)
OUT_DIR.mkdir(exist_ok=True)

# extract all .in files
with zipfile.ZipFile(ZIP_FILE, 'r') as z:
    z.extractall(TMP_DIR)

for idx, in_path in enumerate(TMP_DIR.rglob('*.in')):
    lines = in_path.read_text().splitlines()
    L, R, E = map(int, lines[0].split())
    N = L + R + 2
    M = L + E + R
    s = L + R
    t = L + R + 1
    # build edges
    edges = []
    # source to left
    edges += [f"{s} {u} 1" for u in range(0, L)]
    # left to right based on pairs
    edges += [f"{u-1} {L + (v-1)} 1" for u, v in (map(int, line.split()) for line in lines[1:])]
    # right to sink
    edges += [f"{L + v} {t} 1" for v in range(0, R)]

    out_file = OUT_DIR / f"schooldance-{idx}.in"
    out_file.write_text('\n'.join([f"{N} {M} {s} {t}"] + edges) + '\n')

# cleanup
for f in TMP_DIR.iterdir():
    f.unlink()
TMP_DIR.rmdir()