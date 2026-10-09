import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Estado {
    private String nombre;
    private boolean inicial;
    private boolean fin;
    private Map<String, List<Estado>> transiciones;
    public Estado(String nombre, boolean inicial, boolean fin) {
        this.nombre = nombre;
        this.inicial = inicial;
        this.fin = fin;
        this.transiciones = new HashMap<>();
    }

    public void addTransicion(String simbolo, Estado destino) {
        // Si símbolo aparece por primera vez se crea entrada mapa
        this.transiciones.putIfAbsent(simbolo, new ArrayList<>());

        // Añadir transiciones del símbolo
        this.transiciones.get(simbolo).add(destino);
    }

    // Devuelve la lista de destinos, o una lista vacía si no hay camino con ese símbolo
    public List<Estado> getDestinos(String simbolo) {
        return this.transiciones.getOrDefault(simbolo, new ArrayList<>());
    }

    // --- GETTERS Y SETTERS ---

    public String getNombre() {
        return nombre;
    }

    public boolean isInicial() {
        return inicial;
    }

    public void setInicial(boolean inicial) {
        this.inicial = inicial;
    }

    public boolean isFin() {
        return fin;
    }

    public void setFin(boolean fin) {
        this.fin = fin;
    }

    // Sobrescribimos el toString para que al imprimir el objeto salga su nombre
    @Override
    public String toString() {
        return this.nombre;
    }
}