"""
Serial Debug - Inspecciona los datos que llegan del puerto.
"""

import serial
import serial.tools.list_ports
import sys
import time

PUERTO = 'COM7'
BAUDRATE = 115200
TIMEOUT = 2

def main():
    try:
        ser = serial.Serial(PUERTO, BAUDRATE, timeout=TIMEOUT)
        print(f"✓ Conectado a {PUERTO} a {BAUDRATE} bps\n")
        print("Leyendo 500 bytes con análisis de patrones...\n")
        
        bytes_recibidos = ser.read(500)
        
        print(f"Total: {len(bytes_recibidos)} bytes recibidos\n")
        
        # Mostrar en formato raw
        print("="*60)
        print("DATOS COMO BYTES:")
        print("="*60)
        print(bytes_recibidos)
        
        print("\n" + "="*60)
        print("DATOS COMO LISTA DE DECIMALES:")
        print("="*60)
        print(list(bytes_recibidos))
        
        # Análisis de patrones
        print("\n" + "="*60)
        print("ANÁLISIS DE PATRONES:")
        print("="*60)
        
        # Contar bytes nulos
        nulls = sum(1 for b in bytes_recibidos if b == 0)
        print(f"Bytes nulos (0x00): {nulls}")
        
        # Encontrar primer byte no nulo
        first_nonzero = next((i for i, b in enumerate(bytes_recibidos) if b != 0), -1)
        print(f"Primer byte no nulo en posición: {first_nonzero}")
        
        if first_nonzero >= 0:
            print(f"Valor: 0x{bytes_recibidos[first_nonzero]:02X} ({bytes_recibidos[first_nonzero]})")
        
        # Buscar patrones conocidos
        print("\n" + "="*60)
        print("BÚSQUEDA DE PATRONES:")
        print("="*60)
        
        # Buscar 0x0F (15)
        if 0x0F in bytes_recibidos:
            pos = bytes_recibidos.index(0x0F)
            print(f"✓ Encontrado byte 0x0F en posición {pos}")
        else:
            print("✗ Byte 0x0F (15) no encontrado")
        
        # Buscar 0xB5 (181)
        if 0xB5 in bytes_recibidos:
            pos = bytes_recibidos.index(0xB5)
            print(f"✓ Encontrado byte 0xB5 en posición {pos}")
        else:
            print("✗ Byte 0xB5 no encontrado")
        
        # Buscar secuencia [10, 10] (footer esperado)
        if bytes_recibidos.count(bytes([10, 10])) > 0:
            print(f"✓ Encontrado footer [10, 10]")
        else:
            print("✗ Footer [10, 10] no encontrado")
        
        # Mostrar tabla hex de los primeros 100 bytes
        print("\n" + "="*60)
        print("TABLA HEX (primeros 100 bytes):")
        print("="*60)
        print("Pos | Hex  | Dec | Char")
        print("-" * 30)
        for i in range(min(100, len(bytes_recibidos))):
            byte = bytes_recibidos[i]
            char_repr = chr(byte) if 32 <= byte < 127 else '.'
            print(f"{i:3d} | 0x{byte:02X} | {byte:3d} | '{char_repr}'")
        
        ser.close()
        print("\n✓ Conexión cerrada")
        
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == '__main__':
    main()

