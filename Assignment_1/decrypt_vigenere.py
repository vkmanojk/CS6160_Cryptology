import os
import sys
from math import sqrt
from collections import Counter
from string import ascii_lowercase

L = 25 # Maximum possible key length

cipher =  'DVNSJ QRVPC GWBVZ QGVVP VILQJ VTURP BNPZN TGQIG SIGVW MTCIM FQUIA CCKVG CBYKQ \
            ZNTNB UCIEN UHIVF IWPQC BNKCM IGGGV OEWEV PVGFX APQKM EATDR TBIQG IPRNX JECGQ \
            NPLMY EDURF IPRXX AVVDZ JKIPN UBQYG PVQGM XYCXV RFIPN VIPRI GMNVT AGVGM NUJZR \
            KCBUG QCVNS QAILI FPDBN UXVTN TJBQZ JHVIP RMCWJ NTLTG IPNVR WAPTK GGSBU GBIYN \
            TDRTN ZBQBK BPIIV PTLZC CCFEG QCVHW AFXNS GGMAV HCOLT KGUXV PNJLV PVUNV WMZCI \
            QPUPA GTDVB ONXUK AWFQE PLOTL VEXVR GCOVP TMEKC ONPSP VUIWE AIPRU RPBNP ZQGRQ \
            QGSBB UEMAF IPRFP GRZET BTXVT GKMEA RWEPT ZBHIP RNXJE CGGGJ TNVTH BEQDU PQCBN \
            KCMQD DWXUD VZCIP ROPBV EHBUG HKUQA IETTI QCQWH VCCZD TZFIT WZGIZ VEUQT WGMFC \
            CLRNT ONPIX EQDNF VWIGJ PLVPH XVTTL TGCME CIQBP HWSVW QAMTZ FQCMZ CCCFE GQCVS \
            MFEGQ OGSPB YHQZR AMBDH MEXPB VQCAP QJTQN TIQVD LRGEB UGDZR OHEUK AMNPD BUGGM \
            KRAIV PTLUQ LIOUI ZNEIQ QGPAB HIMAH DCAFE ZNEIQ PCAIC RAQPC IQBPH KRPIC EKTAY \
            CIMEV WMFEW WYCGZ RCAQF GSBUC IUNVW MZCIQ PULIF PDBZG GMYAP KBNAM PVXWA QUNBT \
            BCYCH JHVPT NPVCN ITNBT SMFEG QOKCO CCIBR TCANP SZRCH WAKCO NDDCG VWMJQ GTQ'

english = [
    8.05,  # a
    1.54,  # b
    3.20,  # c
    3.65,  # d
    12.31, # e
    2.28,  # f
    1.96,  # g
    5.14,  # h
    7.18,  # i
    0.16,  # j
    0.77,  # k
    4.03,  # l
    2.25,  # m
    7.19,  # n
    7.94,  # o
    2.29,  # p
    0.12,  # q
    6.03,  # r
    6.59,  # s
    9.59,  # t
    3.10,  # u
    0.98,  # v
    2.56,  # w
    0.14,  # x
    2.00,  # y
    0.09   # z
]

def ioc(text):
    text = ''.join(text.lower().split())
    N = len(text)
    if N < 2:
        return 0
    counts = Counter(text)
    num = 26 * sum(n*(n-1) for n in counts.values())
    den = N * (N-1)
    return num / den

def cosine(obs, exp):
    dot = sum(o * e for o, e in zip(obs, exp))
    norm_obs = sqrt(sum(o * o for o in obs))
    norm_exp = sqrt(sum(e * e for e in exp))
    if norm_obs==0 or norm_exp==0:
        return 0
    return dot / (norm_obs * norm_exp)

def shift_cipher(key):
    msg = ''
    for i, c in enumerate(cipher):
        if c.isalpha():
            shift = ord(key[i % len(key)])  - ord('a')
            msg += chr((ord(c) - ord('a') - shift) % 26 + ord('a'))
    return msg

def display_text(key, msg):
    print(f"Key: {key.upper()}\n")
    print("Plain Text:")
    for i, c in enumerate(msg.upper()):
        print(c, end='')
        if (i + 1) % 5 == 0:
            print(' ', end='')
        if (i + 1) % 65 == 0:
            print()
    if len(msg) % 65 != 0:
        print()

    os.makedirs("output", exist_ok=True)
    with open("output/decrypted_vigenere.txt", "w") as f:
        f.write(msg)

def decrypt_vigenere(slices):
    key = ''
    for slice in slices:
        best_shift = 0
        best_score = -1
        for shift in range(26):
            shifted = ''.join(chr((ord(c) - ord('a') - shift) % 26 + ord('a')) for c in slice)
            counts = Counter(shifted)
            N = len(shifted)
            obs = [counts.get(c,0)/N for c in ascii_lowercase]
            exp = [e/100 for e in english]
            score = cosine(obs, exp)
            if score > best_score:
                best_score = score
                best_shift = shift
        key += chr(best_shift + ord('a'))
    display_text(key, shift_cipher(key))

if __name__ == '__main__':
    if len(sys.argv) > 1:
        try:
            with open(sys.argv[1], 'r') as f:
                cipher = f.read()
        except OSError as e:
            print(f'Error: Could not open file: {e}')

    if len(sys.argv) > 2:
        try:
            L = int(sys.argv[2])
            if L < 1:
                raise ValueError
        except ValueError:
            print("Error: Maximum key length must be a positive integer.")
            sys.exit(1)

    cipher = ''.join(cipher.lower().split())
    best_diff = float('inf')
    for i in range(1, L+1):
        slices = [cipher[j::i] for j in range(i)]
        avg = sum(ioc(k) for k in slices) / len(slices)
        print(f'Key length {i}: Average IoC = {avg:.2f}')
        diff = abs(avg - 1.7)
        if diff < best_diff:
            best_diff = diff
            best_slices = slices
    decrypt_vigenere(best_slices)