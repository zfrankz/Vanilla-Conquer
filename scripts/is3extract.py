#!/usr/bin/env python3
"""Minimal InstallShield 3 (.Z) archive lister/extractor.

Format reference: wfr/unshieldv3 (ISArchiveV3.cpp). Decompression is
PKWARE DCL "implode", reimplemented here after zlib's contrib/blast.c.

usage: is3extract.py list ARCHIVE
       is3extract.py extract ARCHIVE DESTDIR [NAME ...]
"""
import os
import struct
import sys

UNCOMPRESSED = 0x10


def read_toc(data):
    sig1, sig2 = struct.unpack_from("<II", data, 0)
    if sig1 != 0x8C655D13 or sig2 != 0x02013A:
        raise ValueError("not an InstallShield 3 archive")
    toc = struct.unpack_from("<I", data, 41)[0]
    dir_count = struct.unpack_from("<H", data, 49)[0]
    pos = toc
    dirs = []
    for _ in range(dir_count):
        file_count, chunk, name_len = struct.unpack_from("<HHH", data, pos)
        name = data[pos + 6:pos + 6 + name_len].decode("latin-1")
        dirs.append((name, file_count))
        pos += chunk
    files = []
    for dname, count in dirs:
        for _ in range(count):
            usize, csize, offset = struct.unpack_from("<III", data, pos + 3)
            chunk = struct.unpack_from("<H", data, pos + 23)[0]
            attrib = data[pos + 25]
            name_len = data[pos + 29]
            name = data[pos + 30:pos + 30 + name_len].decode("latin-1")
            full = dname + "\\" + name if dname else name
            if ".." in full.replace("\\", "/").split("/"):
                raise ValueError("unsafe path: " + full)
            files.append(dict(path=full, usize=usize, csize=csize, offset=offset, attrib=attrib))
            pos += chunk
    return files


class Huffman:
    def __init__(self, compact, n):
        lengths = []
        for b in compact:
            lengths += [b & 15] * ((b >> 4) + 1)
        assert len(lengths) == n
        self.count = [0] * 14
        for l in lengths:
            self.count[l] += 1
        offs = [0] * 14
        for l in range(1, 13):
            offs[l + 1] = offs[l] + self.count[l]
        self.symbol = [0] * n
        for sym, l in enumerate(lengths):
            if l:
                self.symbol[offs[l]] = sym
                offs[l] += 1


LENLEN = Huffman([2, 35, 36, 53, 38, 23], 16)
DISTLEN = Huffman([2, 20, 53, 230, 247, 151, 248], 64)
BASE = [3, 2, 4, 5, 6, 7, 8, 9, 10, 12, 16, 24, 40, 72, 136, 264]
EXTRA = [0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8]


def explode(src):
    pos = 0
    bitbuf = 0
    bitcnt = 0
    out = bytearray()

    def bits(n):
        nonlocal pos, bitbuf, bitcnt
        while bitcnt < n:
            bitbuf |= src[pos] << bitcnt
            pos += 1
            bitcnt += 8
        v = bitbuf & ((1 << n) - 1)
        bitbuf >>= n
        bitcnt -= n
        return v

    def decode(h):
        # Codes are stored bit-inverted, most significant bit first.
        code = first = index = 0
        for l in range(1, 14):
            code |= bits(1) ^ 1
            count = h.count[l]
            if code < first + count:
                return h.symbol[index + code - first]
            index += count
            first = (first + count) << 1
            code <<= 1
        raise ValueError("bad huffman code")

    lit = bits(8)
    dict_bits = bits(8)
    if lit != 0:
        raise ValueError("coded literals not supported")
    if dict_bits not in (4, 5, 6):
        raise ValueError("bad dictionary size")
    while True:
        if bits(1):
            sym = decode(LENLEN)
            length = BASE[sym] + bits(EXTRA[sym])
            if length == 519:
                break
            shift = 2 if length == 2 else dict_bits
            dist = (decode(DISTLEN) << shift) + bits(shift) + 1
            if dist > len(out):
                raise ValueError("distance too far back")
            for _ in range(length):
                out.append(out[-dist])
        else:
            out.append(bits(8))
    return bytes(out)


def main(argv):
    cmd, archive = argv[1], argv[2]
    with open(archive, "rb") as f:
        data = f.read()
    files = read_toc(data)
    if cmd == "list":
        for e in files:
            print(f"{e['usize']:>10} {e['csize']:>10} {e['path']}")
        return 0
    dest = argv[3]
    wanted = {n.upper() for n in argv[4:]}
    ok = True
    for e in files:
        if wanted and e["path"].upper() not in wanted:
            continue
        raw = data[e["offset"]:e["offset"] + e["csize"]]
        body = raw if e["attrib"] & UNCOMPRESSED else explode(raw)
        status = "OK" if len(body) == e["usize"] else f"SIZE MISMATCH ({len(body)})"
        ok &= status == "OK"
        target = os.path.join(dest, *e["path"].split("\\"))
        os.makedirs(os.path.dirname(target), exist_ok=True)
        with open(target, "wb") as f:
            f.write(body)
        print(f"{status:>4} {e['usize']:>10} {e['path']}")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv))
