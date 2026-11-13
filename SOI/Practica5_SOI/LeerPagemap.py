import os
import sys

pid = int(sys.argv[1])
direccion = int(sys.argv[2], 16)

tam_pagina = os.sysconf("SC_PAGE_SIZE")

numero_pagina = direccion // tam_pagina
offset = numero_pagina * 8

with open(f"/proc/{pid}/pagemap", "rb") as f:
    f.seek(offset)
    entrada = int.from_bytes(f.read(8), "little")

presente = (entrada >> 63) & 1
swap = (entrada >> 62) & 1
exclusiva = (entrada >> 56) & 1
pfn = entrada & ((1 << 55) - 1)

print(f"Direccion virtual: 0x{direccion:x}")
print(f"Tamaño de pagina: {tam_pagina}")
print(f"Entrada pagemap: 0x{entrada:016x}")
print(f"Presente en RAM: {presente}")
print(f"Presente en swap: {swap}")

if presente:
    print(f"Número de página física: {pfn}")

    if pfn != 0:
        direccion_fisica = pfn * tam_pagina + direccion % tam_pagina
        print(f"Direccion fisica: 0x{direccion_fisica:x}")
    else:
        print("PFN no disponible o restringido")