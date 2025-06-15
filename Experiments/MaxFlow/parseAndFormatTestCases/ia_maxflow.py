"""
Reformat Infoarena “Flux maxim” test cases (1-indexed) into 0-indexed format.
Download from: https://infoarena.ro/problema/maxflow?action=attach-list (login required)

Original input:
V E
u_i v_i cap (1-indexed nodes, s=1, t=V)

Reformatted:
V E 0 V-1
u_i v_i cap (0-indexed)
"""

from pathlib import Path
import zipfile

ZIP_FILE = Path(__file__).parents[2] / 'related_problems' / '6_infoarena_max_flow.zip'
TMP_DIR  = Path(__file__).parent / 'tmp'
OUT_DIR  = Path(__file__).parent.parent / 'testCases'

TMP_DIR.mkdir(exist_ok=True)
OUT_DIR.mkdir(exist_ok=True)

with zipfile.ZipFile(ZIP_FILE, 'r') as z:
    z.extractall(TMP_DIR)

for idx, in_path in enumerate(TMP_DIR.rglob('*.in')):
    lines = in_path.read_text().splitlines()
    V, E = map(int, lines[0].split())
    header = f"{V} {E} 0 {V-1}"
    edges = [f"{int(u)-1} {int(v)-1} {cap}" \
             for u, v, cap in (line.split() for line in lines[1:])]
    out_file = OUT_DIR / f"maxflow-{idx}.in"
    out_file.write_text('\n'.join([header] + edges) + '\n')

# cleanup
for f in TMP_DIR.iterdir(): f.unlink()
TMP_DIR.rmdir()