"""
Analizador de patrones para detectar la estructura real del protocolo.
"""

import serial
import struct

PUERTO = 'COM7'
BAUDRATE = 115200

def main():
    ser = serial.Serial(PUERTO, BAUDRATE, timeout=2)
    print("Leyendo datos e intentando detectar patrones...\n")
    
    data = ser.read(500)
    
    # Descartar bytes nulos al inicio
    start_idx = next((i for i, b in enumerate(data) if b != 0), 0)
    data_clean = data[start_idx:]
    
    print(f"Datos válidos (sin nulos iniciales) a partir de byte {start_idx}:")
    print(f"Primeros 100 bytes: {data_clean[:100]}\n")
    
    # Intentar interpretar como int16 (2 bytes cada uno)
    print("="*60)
    print("INTERPRETANDO COMO INT16 (little-endian):")
    print("="*60)
    try:
        values_i16 = struct.unpack(f'<{len(data_clean)//2}h', data_clean[:len(data_clean)//2*2])
        print(f"Primeros 20 valores int16: {values_i16[:20]}\n")
    except:
        print("No se pudo interpretar como int16\n")
    
    # Intentar como uint16
    print("="*60)
    print("INTERPRETANDO COMO UINT16 (little-endian):")
    print("="*60)
    try:
        values_u16 = struct.unpack(f'<{len(data_clean)//2}H', data_clean[:len(data_clean)//2*2])
        print(f"Primeros 20 valores uint16: {values_u16[:20]}\n")
    except:
        print("No se pudo interpretar como uint16\n")
    
    # Buscar patrones repetitivos
    print("="*60)
    print("BÚSQUEDA DE PATRONES REPETITIVOS:")
    print("="*60)
    
    # Buscar bytes que se repiten
    byte_counts = {}
    for b in data_clean:
        byte_counts[b] = byte_counts.get(b, 0) + 1
    
    sorted_bytes = sorted(byte_counts.items(), key=lambda x: x[1], reverse=True)
    print("Bytes más frecuentes:")
    for byte, count in sorted_bytes[:10]:
        pct = 100 * count / len(data_clean)
        print(f"  0x{byte:02X} ({byte:3d}): {count:3d} veces ({pct:5.1f}%)")
    
    # Buscar transiciones (cambios entre bytes)
    print("\n" + "="*60)
    print("PRIMEROS DATOS VÁLIDOS (sin nulos):")
    print("="*60)
    print("Índice | Hex  | Dec  | ASCII")
    print("-" * 40)
    for i, b in enumerate(data_clean[:50]):
        char = chr(b) if 32 <= b < 127 else '.'
        print(f"{i:5d} | 0x{b:02X} | {b:3d}  | '{char}'")
    
    ser.close()

if __name__ == '__main__':
    main()
