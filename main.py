import argparse
from typing import cast

import progressbar
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.kdf.hkdf import HKDF


class ProgressBar:
    bar: progressbar.ProgressBar
    num: int = 0

    def __init__(self, maxval: int) -> None:
        widgets = [progressbar.Percentage(), " ", progressbar.Bar(), " ", progressbar.Counter(), f"/{maxval} ", progressbar.AdaptiveETA()]
        self.bar = progressbar.ProgressBar(maxval=maxval, widgets=widgets)

    def start(self) -> None:
        self.bar.start()

    def stop(self) -> None:
        self.bar.finish()

    def update(self) -> None:
        self.num += 1
        self.bar.update(self.num)


def main():
    args = parse_arguments()

    target_specifier, target_hash = args.target
    target_hash = cast(bytes, target_hash)
    target_specifier = cast(bytes, target_specifier)
    wordlist: str = args.wordlist

    print("[-] Counting words in wordlist...")
    line_count = count_lines(wordlist)
    print(f"[-] Found {line_count} words")

    bar = ProgressBar(line_count)

    bar.start()
    with open(wordlist, "rb") as file:
        for line in file.readlines():
            line = line.rstrip()
            calculated_hash = secret_to_key_rfc2440(line, target_specifier)
            bar.update()
            if calculated_hash == target_hash:
                bar.stop()
                print(f"[+] Found password: {line.decode()}")
                exit(0)

    bar.stop()
    print("[!] Password not in wordlist")
    exit(1)


def parse_arguments() -> argparse.Namespace:
    def target(string: str) -> tuple[bytes, bytes]:
        target = string.rsplit(":", 1)[-1]
        target = bytes.fromhex(target)

        target_specifier = target[0:9]
        target_hash = target[9:]

        return target_specifier, target_hash

    parser = argparse.ArgumentParser(
        description="Execute a dictionary attack on the value of the HashedControlPassword line of a torrc file",
        epilog="Hack the planet!",
    )

    parser.add_argument(
        "-t",
        "--target",
        type=target,
        required=True,
        help="The hash to crack. Looks like 16:46CAB5E3FA5F7BF4606D132C27DA06F77B574F7C14B9D75657A74F59FB",
        metavar="HASH",
    )
    parser.add_argument("-w", "--wordlist", required=True, help="File containing the list of passwords to try", metavar="WORDLIST_FILE")
    return parser.parse_args()


def count_lines(file_name: str) -> int:
    num = 0
    with open(file_name, "rb") as file:
        for line in file.readlines():
            num += 1

    return num


def secret_to_key_rfc2440(secret: bytes, s2k_specifier: bytes) -> bytes:
    key_out_len = 20

    c = s2k_specifier[8]
    count = (16 + (c & 15)) << ((c >> 4) + 6)

    digest = hashes.Hash(hashes.SHA1(), backend=default_backend())

    tmp = bytearray(s2k_specifier[:8]) + bytearray(secret)

    while count > 0:
        if count >= len(tmp):
            digest.update(tmp)
            count -= len(tmp)
        else:
            digest.update(tmp[:count])
            count = 0

    final_digest = digest.finalize()

    if key_out_len <= len(final_digest):
        return final_digest[:key_out_len]
    else:
        hkdf = HKDF(algorithm=hashes.SHA256(), length=key_out_len, salt=None, info=s2k_specifier[:8], backend=default_backend())
        expanded_key = hkdf.derive(final_digest)

        return expanded_key


if __name__ == "__main__":
    main()
