import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;

public class Main {
    static String fichero = "";
    static HashMap<String, Estado> mapaEstados = new HashMap<>();
    static List<String> alfabeto = new ArrayList<>();
    static Set<Estado> estadosActuales = new HashSet<>();
    public static Set<Estado> clausuraE(Set<Estado> iniciales) {
        Set<Estado> clausura = new HashSet<>(iniciales);
        Stack<Estado> pila = new Stack<>();
        pila.addAll(iniciales);

        while (!pila.isEmpty()) {
            Estado actual = pila.pop();
            // La cadena vacía "" representa la transición épsilon
            List<Estado> destinosEpsilon = actual.getDestinos("");
            for (Estado destino : destinosEpsilon) {
                if (!clausura.contains(destino)) {
                    clausura.add(destino);
                    pila.push(destino);
                }
            }
        }
        return clausura;
    }
    public static void simular(String cadenaEntrada) {
        // 1. Limpiamos espacios en blanco al principio y al final
        cadenaEntrada = cadenaEntrada.trim();
        if (cadenaEntrada.isEmpty()) return;

        // Si es la primera ejecución, buscamos el estado inicial
        if (estadosActuales.isEmpty()) {
            for (Estado e : mapaEstados.values()) {
                if (e.isInicial()) {
                    estadosActuales.add(e);
                }
            }
            estadosActuales = clausuraE(estadosActuales);
        }

        System.out.println("--- Iniciando simulación con entrada: " + cadenaEntrada + " ---");
        System.out.println("Estados actuales iniciales: " + estadosActuales);

        String[] simbolos = cadenaEntrada.split("\\s+");

        for (String simbolo : simbolos) {
            // 2. Ignoramos si por cualquier motivo queda un token vacío
            if (simbolo.isEmpty()) continue;

            Set<Estado> siguientes = new HashSet<>();

            for (Estado actual : estadosActuales) {
                List<Estado> destinos = actual.getDestinos(simbolo);
                siguientes.addAll(destinos);
            }

            // Aplicamos la clausura épsilon a los siguientes estados
            estadosActuales = clausuraE(siguientes);

            System.out.println("Símbolo procesado [" + simbolo + "] -> Nuevos estados actuales: " + estadosActuales);
        }

        // Comprobación de aceptación
        boolean aceptada = false;
        for (Estado e : estadosActuales) {
            if (e.isFin()) {
                aceptada = true;
                break;
            }
        }

        if (aceptada) {
            System.out.println("Resultado: Cadena ACEPTADA.");
        } else {
            System.out.println("Resultado: Cadena RECHAZADA.");
        }
    }
    public static void leerFichero() {
        try {
            Path rutaFichero = Paths.get(fichero);
            List<String> lineas = Files.readAllLines(rutaFichero);

            // 1. Procesar Estados Totales (Línea 0)
            String[] estadosTotales = lineas.get(0).split(" ");
            for (int i = 1; i < estadosTotales.length; i++) {
                if (i == 1) {
                    Estado estado = new Estado(estadosTotales[i], true, false);
                    mapaEstados.put(estadosTotales[i], estado);
                } else {
                    Estado estado = new Estado(estadosTotales[i], false, false);
                    mapaEstados.put(estadosTotales[i], estado);
                }
            }

            // 2. Procesar Estados Finales (Línea 1)
            String[] estadosFinales = lineas.get(1).split(" ");
            for (int i = 1; i < estadosFinales.length; i++) {
                String nombreEstado = estadosFinales[i];
                if (mapaEstados.containsKey(nombreEstado)) {
                    mapaEstados.get(nombreEstado).setFin(true);
                }
            }

            // 3. Procesar Alfabeto (Línea 2)
            String[] tokensAlfabeto = lineas.get(2).split(" ");
            for (int i = 1; i < tokensAlfabeto.length; i++) {
                alfabeto.add(tokensAlfabeto[i]);
            }

            // 4. Procesar Transiciones (A partir de la línea 4)
            for (int i = 4; i < lineas.size(); i++) {
                String linea = lineas.get(i).trim();
                if (linea.isEmpty()) continue;

                // El estado actual según el orden de declaración en la primera línea
                String nombreEstadoActual = estadosTotales[i - 3];
                Estado estadoActual = mapaEstados.get(nombreEstadoActual);

                String[] tokens = linea.split("\\s+");
                int tokenIndex = 0;

                // El número de columnas es el tamaño del alfabeto + 1 (para la cadena vacía)
                int totalColumnas = alfabeto.size() + 1;

                for (int col = 0; col < totalColumnas; col++) {
                    // Si estamos en la última columna, el símbolo es la cadena vacía ("")
                    String simbolo = (col < alfabeto.size()) ? alfabeto.get(col) : "";

                    // Leemos todos los estados destino hasta encontrar el "#" que cierra la columna
                    while (tokenIndex < tokens.length && !tokens[tokenIndex].equals("#")) {
                        String nombreDestino = tokens[tokenIndex];
                        Estado estadoDestino = mapaEstados.get(nombreDestino);

                        if (estadoDestino != null) {
                            estadoActual.addTransicion(simbolo, estadoDestino);
                        }
                        tokenIndex++;
                    }
                    // Saltamos el símbolo "#" para avanzar a la siguiente columna
                    tokenIndex++;
                }
            }

            System.out.println("¡Fichero leído y autómata cargado con éxito!");

        } catch (IOException e) {
            System.out.println("Error con la lectura del fichero: " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        if (args.length == 0) {
            System.out.println("No hay ningún fichero especificado");
            return;
        }

        fichero = args[0];
        leerFichero();

        Scanner scanner = new Scanner(System.in);
        System.out.println("\n==========================================");
        System.out.println(" SIMULADOR DE AUTÓMATA / MÁQUINA EXPENDEDORA");
        System.out.println(" (Escribe 'salir' para terminar el programa)");
        System.out.println("==========================================");

        while (true) {
            System.out.print("\nIntroduce la secuencia de entrada (ej: 1 1 2 o c): ");
            String entrada = scanner.nextLine();

            if (entrada.equalsIgnoreCase("salir")) {
                System.out.println("¡Hasta luego!");
                break;
            }

            if (!entrada.trim().isEmpty()) {
                simular(entrada);
            }
        }
        scanner.close();
    }
}