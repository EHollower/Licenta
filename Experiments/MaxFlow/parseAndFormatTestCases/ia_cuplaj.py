"""
Reformat Infoarena “Cuplaj maxim in graf bipartit” test cases (1-indexed bipartite matching) into maxflow inputs.
Download from: https://infoarena.ro/problema/cuplaj?action=attach-list (login required).

Original input:
 L R E
 u_i v_i  (1 ≤ u ≤ L, 1 ≤ v ≤ R)

Reformatted:
 N M s t
 edges:
  s → left nodes (cap=1)
  left u_i → right (v_i+L) (cap=1)
  right nodes → t (cap=1)
where N=L+R+2, M=L+E+R, s=L+R, t=L+R+1.
Only include cases with N <= 10000.
"""

from pathlib import Path
import zipfile

ZIP_FILE = Path(__file__).parents[2] / 'related_problems' / '7_infoarena_cuplaj.zip'
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
    if N > 10000:
        continue
    M = L + E + R
    s = L + R
    t = L + R + 1
    edges = []
    # source to left
    edges += [f"{s} {i} 1" for i in range(L)]
    # left to right
    edges += [f"{u-1} {L + (v-1)} 1"
               for u, v in (map(int, line.split()) for line in lines[1:])]
    # right to sink
    edges += [f"{L + j} {t} 1" for j in range(R)]

    out_file = OUT_DIR / f"cuplaj-{idx}.in"
    out_file.write_text('\n'.join([f"{N} {M} {s} {t}"] + edges) + '\n')

# cleanup
for f in TMP_DIR.iterdir(): f.unlink()
TMP_DIR.rmdir()