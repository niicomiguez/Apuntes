#!/bin/bash

# Lista de departamentos
DEPARTAMENTOS=("gerencia" "administracion" "programacion" "sistemas" "comercial" "rrhh" "contabilidad")

echo "=== 1. Creando grupos ==="
# Crear un grupo por cada departamento
for dept in "${DEPARTAMENTOS[@]}"; do
    groupadd "$dept" 2>/dev/null
done

# Crear grupos adicionales para jefes y empleados
groupadd jefes 2>/dev/null
groupadd empleados 2>/dev/null

echo "=== 2 y 3. Creando directorios de departamentos y zonas work ==="
for dept in "${DEPARTAMENTOS[@]}"; do
    # Crear directorios del departamento y subdirectorio work
    mkdir -p "/home/$dept/work"

    # Permisos para /home/{departamento}: Propietario root:departamento, solo entran usuarios de ese grupo (750)
    chown root:"$dept" "/home/$dept"
    chmod 750 "/home/$dept"

    # Permisos para /home/{departamento}/work: Propietario root:departamento, miembros crean y borran (770)
    chown root:"$dept" "/home/$dept/work"
    chmod 770 "/home/$dept/work"
done

echo "=== 4. Creando directorios comunes ==="
# Crear directorio común y el subdirectorio exclusivo para jefes
mkdir -p /home/comun/comunJefes

# /home/comun: Acceso total para escribir, pero solo el propietario/root borra (Sticky bit 1777)
chown root:root /home/comun
chmod 1777 /home/comun

# /home/comun/comunJefes: Exclusivo jefes (grupo jefes) con Sticky bit (1770)
chown root:jefes /home/comun/comunJefes
chmod 1770 /home/comun/comunJefes

echo "Estructura de grupos y directorios creada correctamente."
