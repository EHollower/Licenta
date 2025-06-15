"""
Reformat LOJ6002 test cases (bipartite matching) into maxflow inputs.
Download from: https://loj.ac/p/6002/files (login required).

Original input:
 N E
 u_i v_i  (1-indexed: u in [1..L], v in [1..R])

Reformatted:
 N M s t
 edges:
   s→each left node (cap=1)
   left u_i→right (L+v_i) (cap=1)
   each right node→t (cap=1)
where N=L+R+2, M=L+E+R, s=L+R, t=L+R+1.
"""

from pathlib import Path
import zipfile

ZIP_FILE = Path(__file__).parents[2] / 'related_problems' / '10_loj_6002.zip'
TMP_DIR  = Path(__file__).parent / 'tmp'
OUT_DIR  = Path(__file__).parent.parent / 'testCases'

TMP_DIR.mkdir(exist_ok=True)
OUT_DIR.mkdir(exist_ok=True)

with zipfile.ZipFile(ZIP_FILE, 'r') as z:
    z.extractall(TMP_DIR)

for idx, in_path in enumerate(TMP_DIR.rglob('*.in')):
    lines = in_path.read_text().splitlines()
    N, E = map(int, lines[0].split())
    L, R = N, N
    N = L + R + 2
    s = L + R
    t = L + R + 1
    edges = []
    # source→left
    edges += [f"{s} {i} 1" for i in range(L)]
    # left→right based on input edges
    edges += [f"{u-1} {L + (v-1)} 1"
             for u, v in (map(int, line.split()) for line in lines[1:])]
    # right→sink
    edges += [f"{L + j} {t} 1" for j in range(R)]
    header = f"{N} {len(edges)} {s} {t}"
    out_file = OUT_DIR / f"loj6002-{idx}.in"
    out_file.write_text('\n'.join([header] + edges) + '\n')

# cleanup
for f in TMP_DIR.iterdir(): f.unlink()
TMP_DIR.rmdir()