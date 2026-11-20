#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import grp
import os
import pwd
import random
import string
import subprocess
import sys
from passlib.hash import md5_crypt


class CustomParser(argparse.ArgumentParser):
    def error(self, message):
        self.print_usage(sys.stderr)
        self.exit(2, f"{self.prog}: error: {message}\n\n")


def generar_clave_aleatoria(longitud=8):
    """Genera una contraseña aleatoria de 8 caracteres."""
    caracteres = string.ascii_letters + string.digits + ":;,.<>-_+*?=!@#$"
    return "".join(random.choice(caracteres) for _ in range(longitud))


def main():
    parser = CustomParser(description="Crea usuarios")
    parser.add_argument("-u", help="usuario")
    parser.add_argument("-d", help="departamento")
    parser.add_argument("-p", help="clave")

    grupo_rol = parser.add_mutually_exclusive_group()
    grupo_rol.add_argument("-j", action="store_true", help="jefe")
    grupo_rol.add_argument("-e", action="store_true", help="empleado")

    args = parser.parse_args()

    # Comprobar que se han indicado usuario y departamento
    if not args.u or not args.d:
        print("Error. No se ha indicado usuario y/o departamento")
        parser.print_help()
        sys.exit(1)

    # Comprobar si el usuario ya existe
    try:
        pwd.getpwnam(args.u)
        print(f"Error. El usuario {args.u} existe\n")
        sys.exit(1)
    except KeyError:
        pass

    # Comprobar que el departamento (grupo y directorio) existe
    try:
        grp.getgrnam(args.d)
    except KeyError:
        print(f"Error al crear el usuario {args.u}\n")
        sys.exit(1)

    dir_departamento = os.path.join("/home", args.d)
    if not os.path.isdir(dir_departamento):
        print(f"Error al crear el usuario {args.u}\n")
        sys.exit(1)

    # Datos del usuario
    clave = args.p if args.p else generar_clave_aleatoria()
    clave_encriptada = md5_crypt.hash(clave)
    home_usuario = os.path.join(dir_departamento, args.u)
    gecko = f"{args.u},{args.d}"
    grupo_secundario = "jefes" if args.j else "empleados"

    # Crear usuario con useradd
    cmd = [
        "useradd",
        "-m",
        "-d",
        home_usuario,
        "-g",
        args.d,
        "-G",
        grupo_secundario,
        "-c",
        gecko,
        "-s",
        "/bin/bash",
        "-p",
        clave_encriptada,
        args.u,
    ]

    try:
        subprocess.run(
            cmd,
            check=True,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
        )
    except subprocess.CalledProcessError:
        print(f"Error al crear el usuario {args.u}\n")
        sys.exit(1)

    os.chmod(home_usuario, 0o755)

    print("Usuario creado satisfactoriamente")
    print(f"{'user:':<7}{args.u}")
    print(f"{'dep:':<7}{args.d}")
    print(f"{'clave:':<7}{clave}")
    print(f"{'home:':<7}{home_usuario}")
    print(f"{'gecko:':<7}{gecko}")
    print(f"{'grupo:':<7}{grupo_secundario}")

    # Configurar opciones de caducidad de la contraseña
    try:
        subprocess.run(
            ["chage", "-M", "90", "-m", "2", "-W", "7", args.u],
            check=True,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
        )
        print("opciones de la contrasena puestas correctamente")
    except subprocess.CalledProcessError:
        print(f"Error al configurar la contrasena de {args.u}\n")
        sys.exit(1)

    # Crear enlaces simbólicos en el directorio personal del usuario
    destino_work = f"/home/{args.d}/work"
    os.symlink(destino_work, os.path.join(home_usuario, "work"))
    print(f"creado {destino_work}")

    destino_comun = "/home/comun"
    os.symlink(destino_comun, os.path.join(home_usuario, "comun"))
    print(f"creado {destino_comun}")

    if args.j:
        destino_jefes = "/home/comun/comunJefes"
        os.symlink(destino_jefes, os.path.join(home_usuario, "comunJefes"))
        print(f"creado {destino_jefes}")

    print()


if __name__ == "__main__":
    main()
