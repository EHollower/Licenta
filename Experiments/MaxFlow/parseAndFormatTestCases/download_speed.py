"""
Reformat CSES “Download Speed” test cases (1-indexed) into 0-indexed format.
Download the test cases from: https://cses.fi/problemset/tests/1694/ (login required)

Original input:
V E
u_i v_i cap_i

Reformatted:
V E s t
u_i v_i cap_i
where s = 0, t = V - 1.
"""

from pathlib import Path
import zipfile

# paths
ZIP_FILE = Path(__file__).parents[2] / 'related_problems' / '1_download_speed.zip'
TMP_DIR  = Path(__file__).parent / 'tmp'
OUT_DIR  = Path(__file__).parent.parent / 'testCases'

TMP_DIR.mkdir(exist_ok=True)
OUT_DIR.mkdir(exist_ok=True)

# extract all .in files into tmp
with zipfile.ZipFile(ZIP_FILE, 'r') as z:
    z.extractall(TMP_DIR)

# format and write all .in files
for idx, in_path in enumerate(TMP_DIR.rglob('*.in')):
    lines = in_path.read_text().splitlines()
    V, E = map(int, lines[0].split())
    header = f"{V} {E} 0 {V-1}"
    edges = [f"{int(u)-1} {int(v)-1} {cap}" \
             for u, v, cap in (line.split() for line in lines[1:])]
    out_file = OUT_DIR / f"downloadspeed-{idx}.in"
    out_file.write_text('\n'.join([header] + edges) + '\n')

# cleanup
for f in TMP_DIR.iterdir():
    f.unlink()
TMP_DIR.rmdir()
