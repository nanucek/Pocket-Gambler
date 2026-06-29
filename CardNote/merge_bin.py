import os

# Cesty k tvým souborům (PlatformIO standard pro Cardputer)
build_dir = ".pio/build/m5stack-stamps3"

files_to_merge = [
    (0x0000, os.path.join(build_dir, "bootloader.bin")),
    (0x8000, os.path.join(build_dir, "partitions.bin")),
    (0x10000, os.path.join(build_dir, "firmware.bin"))
]

output_file = os.path.join(build_dir, "firmware_merged.bin")

def merge_binaries():
    print("Začínám spojovat binární soubory pro M5Burner...")
    
    # 1. Zjištění celkové velikosti výsledného souboru
    max_size = 0
    for offset, filepath in files_to_merge:
        if os.path.exists(filepath):
            max_size = max(max_size, offset + os.path.getsize(filepath))
        else:
            print(f"❌ CHYBA: Soubor {filepath} chybí! Zkompiluj projekt znovu.")
            return

    # 2. Vytvoření prázdného pole vyplněného 0xFF (Klíčová oprava!)
    merged_data = bytearray([0xFF] * max_size)

    # 3. Zápis jednotlivých souborů na správné adresy
    for offset, filepath in files_to_merge:
        with open(filepath, 'rb') as f:
            data = f.read()
            merged_data[offset:offset+len(data)] = data
            print(f"✅ Vloženo: {os.path.basename(filepath)} na adresu {hex(offset)}")

    # 4. Uložení výsledného souboru
    with open(output_file, 'wb') as f:
        f.write(merged_data)
        
    print(f"🎉 Hotovo! Správný soubor byl uložen: {output_file}")

if __name__ == "__main__":
    merge_binaries()
    
