
#include <algorithm>
#include <chrono>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>
#include <thread>
#include <vector>
#include <map>

using namespace std;

// Estructuras para almacenar informacion de usuarios
struct Usuario {
    string usuario, nombre, rol;
    string contrasena;
    bool bloqueado = false;
};

struct UsuarioBloqueado {
    string usuario;
    UsuarioBloqueado(const string& usr) : usuario(usr) {}
};

// Vector para almacenar usuarios
vector<Usuario> usuarios;
vector<UsuarioBloqueado> usuariosBloqueados;

bool isAdminLoggedIn = false;
string rolUltimoUsuario = "";

void limpiarPantalla() {
    // Comando específico para limpiar la pantalla en Windows
#ifdef _WIN32
    system("cls");
#endif

    // Comando específico para limpiar la pantalla en sistemas Unix (Linux/macOS)
#ifdef __unix__
    system("clear");
#endif
}

// Procedimiento para inicializar usuarios 
void inicializarUsuarios() {
    usuarios.push_back(
        { "Admin", "Administrador", "Administrador", "admin1234", false });

}

// Procedimiento para el inicio de sesion
bool iniciarSesion() {

    for (int intentos = 0; intentos < 3; ++intentos) {

        system("color 07");
        isAdminLoggedIn = false;
        string usuario;
        string contrasena = "";

        system("color 07");
        limpiarPantalla();
        cout << "====== Inicio de sesion ======" << endl;
        cout << "Ingrese su usuario: ";
        cin >> usuario;

        // Solicitar la contraseña y mostrar asteriscos
        cout << "Ingrese su contrasenna: ";
        char ch;
        while ((ch = _getch()) != '\r') {
            if (ch == '\b') {
                if (!contrasena.empty()) {
                    contrasena.pop_back();
                    cout << "\b \b";
                }
            }
            else {
                contrasena += ch;
                cout << '*';
            }
        }
        cout << endl;

        // Verificar si el usuario es el administrador
        if (usuario == "Admin" && contrasena == "admin1234") {
            cout << "Inicio de sesion exitoso. Bienvenido, Administrador." << endl;
            isAdminLoggedIn = true; // Establecer la variable isAdminLoggedIn a true
            cout << "Presione cualquier tecla para continuar..." << endl;
            _getch();
            return true;
        }

        // Verificar las credenciales del usuario en el vector de usuarios
        for (auto u : usuarios) {
            if (u.usuario == usuario && u.contrasena == contrasena && !u.bloqueado) {
                isAdminLoggedIn = false;
                cout << "Inicio de sesion exitoso. Bienvenido, " << u.rol << " "
                    << u.nombre << "." << endl;
                rolUltimoUsuario = u.rol;
                cout << "Presione cualquier tecla para continuar..." << endl;
                _getch();
                return true;
            }
        }

        for (auto u : usuariosBloqueados) {
            cout << "El usuario esta bloqueado." << endl;
            break;
        }

        // Bloquear las credenciales después del tercer intento fallido

        if (intentos == 2) {
            for (auto& u : usuarios) {
                if (usuario == "Admin") {
                    cout << "El usuario Admin no puede ser bloqueado." << endl;
                }
                else {
                    cout << "El usuario " << usuario << " ha sido bloqueado por demasiados intentos fallidos." << endl;
                    usuariosBloqueados.push_back(UsuarioBloqueado(usuario));
                    u.bloqueado = true;
                    break;
                }
            }
        }

        cout << "Credenciales incorrectas. Intentos restantes: " << 2 - intentos
            << endl;
        cout << "Presione cualquier tecla para continuar..." << endl;
        _getch();
    }

    system("color 47");
    cout << "Demasiados intentos fallidos." << endl;
    cout << "Desea intentarlo de nuevo? (Si: 1 / No: 0): ";
    int opcion;
    cin >> opcion;

    if (opcion == 1) {
        return iniciarSesion(); // Volver a intentar iniciar sesión
    }
    else {
        cout << "Saliendo del programa." << endl;
        isAdminLoggedIn = false; // Restablecer la variable isAdminLoggedIn a false
        return false;
    }
}

void mostrarUsuariosBloqueados() {
    cout << "=== Usuarios Bloqueados ===" << endl;
    if (usuariosBloqueados.empty()) {
        cout << "No hay usuarios bloqueados en este momento." << endl;
    }
    else {
        cout << "---------------------------" << endl;
        cout << "Usuarios Bloqueados:" << endl;
        cout << "---------------------------" << endl;
        int count = 1;
        for (const auto& usuario : usuariosBloqueados) {
            cout << count << ". " << usuario.usuario << endl;
            count++;
        }
        cout << "---------------------------" << endl;
    }
}

// Procedimiento para desbloquear usuarios
void desbloquearUsuario() {
    do {
        limpiarPantalla();
        cout << "=== Desbloquear Usuario ===" << endl;
        cout << "1. Mostrar usuarios bloqueados" << endl;
        cout << "2. Desbloquear usuario" << endl;
        cout << "3. Regresar al menu anterior" << endl;
        cout << "Ingrese una opcion: ";

        char opcion;
        cin >> opcion;

        switch (opcion) {
        case '1': {
            limpiarPantalla();
            mostrarUsuariosBloqueados();
            cout << "Presione cualquier tecla para continuar..." << endl;
            _getch();
            break;
        }
        case '2': {
            limpiarPantalla();
            mostrarUsuariosBloqueados();

            string usuario;
            cout << "Ingrese el usuario a desbloquear: ";
            cin >> usuario;

            bool encontrado = false;

            // Buscar al usuario en la lista de usuarios bloqueados
            for (auto it = usuariosBloqueados.begin(); it != usuariosBloqueados.end();
                ++it) {
                if (it->usuario == usuario) {
                    // Eliminar al usuario bloqueado de la lista de usuarios bloqueados
                    usuariosBloqueados.erase(it);
                    encontrado = true;

                    // Buscar al usuario en la lista de usuarios y desbloquearlo
                    for (auto& u : usuarios) {
                        if (u.usuario == usuario) {
                            u.bloqueado = false;
                            cout << "Usuario " << usuario << " desbloqueado correctamente."
                                << endl;
                            break;
                        }
                    }

                    cout << "Usuario desbloqueado exitosamente" << endl;
                    break;
                }
            }

            if (!encontrado) {
                cout << "No se encontro ningun usuario bloqueado con ese nombre."
                    << endl;
            }

            cout << "Presione cualquier tecla para continuar..." << endl;
            _getch();
            break;
        }
        case '3': {
            return; // Regresar al menú anterior
        }
        default: {
            cout << "Opcion no valida. Intente nuevamente." << endl;
            cout << "Presione cualquier tecla para continuar..." << endl;
            _getch();
            break;
        }
        }
    } while (true);
}

void gestionarUsuarios() {
    limpiarPantalla();
    cout << "=== Gestion de Usuarios ===" << endl;
    cout << "1. Cambiar contrasena de un usuario" << endl;
    cout << "2. Bloquear/Desbloquear un usuario" << endl;
    cout << "3. Cambiar nombre de usuario" << endl;
    cout << "4. Regresar al menu anterior" << endl;
    cout << "Ingrese una opcion: ";
    char opcion;
    cin >> opcion;

    switch (opcion) {
    case '1': {
        limpiarPantalla();
        string usuario;
        cout << "Ingrese el nombre de usuario: ";
        cin >> usuario;

        // Buscar al usuario en la lista de usuarios
        auto it = find_if(usuarios.begin(), usuarios.end(),
            [&](const Usuario& u) { return u.usuario == usuario; });

        if (it != usuarios.end() &&
            it->usuario !=
            "Admin") { // Verificar que no sea el usuario Administrador
            string nuevaContrasena;
            cout << "Ingrese la nueva contrasena: ";
            cin >> nuevaContrasena;
            it->contrasena = nuevaContrasena;
            cout << "Contrasena actualizada correctamente." << endl;
        }
        else {
            cout << "No se puede cambiar la contrasena del usuario Administrador."
                << endl;
        }

        cout << "Presione cualquier tecla para continuar..." << endl;
        _getch();
        break;
    }
    case '2': {
        limpiarPantalla();
        string usuario;
        cout << "Ingrese el nombre de usuario: ";
        cin >> usuario;

        // Buscar al usuario en la lista de usuarios
        auto it = find_if(usuarios.begin(), usuarios.end(),
            [&](const Usuario& u) { return u.usuario == usuario; });

        if (it != usuarios.end() &&
            it->usuario !=
            "Admin") { // Verificar que no sea el usuario Administrador
            it->bloqueado = !it->bloqueado;
            usuariosBloqueados.push_back(UsuarioBloqueado(usuario));

            cout << "Usuario " << (it->bloqueado ? "bloqueado" : "desbloqueado")
                << " correctamente." << endl;
        }
        else {
            cout << "No se puede bloquear o desbloquear al usuario Administrador."
                << endl;
        }

        cout << "Presione cualquier tecla para continuar..." << endl;
        _getch();
        break;
    }
    case '3': {
        limpiarPantalla();
        string usuario;
        cout << "Ingrese el nombre de usuario actual: ";
        cin >> usuario;

        // Buscar al usuario en la lista de usuarios
        auto it = find_if(usuarios.begin(), usuarios.end(),
            [&](const Usuario& u) { return u.usuario == usuario; });

        if (it != usuarios.end() &&
            it->usuario !=
            "Admin") { // Verificar que no sea el usuario Administrador
            string nuevoUsuario;
            cout << "Ingrese el nuevo nombre de usuario: ";
            cin >> nuevoUsuario;
            it->usuario = nuevoUsuario;
            cout << "Nombre de usuario actualizado correctamente." << endl;
        }
        else {
            cout << "No se puede cambiar el nombre de usuario del Administrador."
                << endl;
        }

        cout << "Presione cualquier tecla para continuar..." << endl;
        _getch();
        break;
    }
    case '4': {
        return; // Regresar al menu anterior
    }
    default: {
        cout << "Opcion no valida. Intente nuevamente." << endl;
        cout << "Presione cualquier tecla para continuar..." << endl;
        _getch();
        break;
    }
    }
}

// Estructura para almacenar información de los smartphones
struct Smartphone {
    string marca;
    string modelo;
    int anioFabricacion;
    string sistemaOperativo;
    string procesador;
    int ram;
    int rom;
    string gama;
    int precio;
    int existencias;
    int camaraFrontal;
    int camaraTrasera;
    float pantalla;
    int numSerie;
};

// Estructura para almacenar información de las laptops
struct Laptop {
    string marca;
    string modelo;
    string sistemaOperativo;
    string procesador;
    int ram;
    struct DiscoDuro {
        string tipo;   // HDD o SSD
        int capacidad; // Capacidad en GB
    } discoDuro;
    int precio;
    float pantalla;
    string tarjetaVideo;
    bool esGamer;
    int existencias;
    int numSerie;
};

// Estructura para almacenar información de las tablets
struct Tablet {
    string marca;
    string modelo;
    string sistemaOperativo;
    string procesador;
    int ram;
    int rom;
    int precio;
    float pantalla;
    int existencias;
    int numSerie;
};

// Vectores para almacenar productos y usuarios
vector<Smartphone> smartphones;
vector<Laptop> laptops;
vector<Tablet> tablets;

// Estructura para la orden
struct OrdenDetalle {
    string producto;
    string marca;
    string modelo;
    int cantidad;
    double precioUnitario;
};

vector<OrdenDetalle> orden;

// Procedimiento para inicializar datos de productos
void inicializarDatos() {

    int numeroSerieSmartphone = 100000;

    // Inicialización de smartphones (25 unidades)
    smartphones.push_back({ "Apple", "iPhone 12", 2020, "iOS", "A14 Bionic", 4,
                           128, "Alta", 800, 11, 12, 12, 6.1,
                           numeroSerieSmartphone++ });
    smartphones.push_back({ "Apple", "iPhone 13", 2021, "iOS", "A15 Bionic", 4,
                           128, "Alta", 900, 11, 12, 12, 6.1,
                           numeroSerieSmartphone++ });
    smartphones.push_back({ "Samsung", "Galaxy S21", 2021, "Android",
                           "Exynos 2100", 8, 256, "Alta", 1000, 15, 10, 64, 6.2,
                           numeroSerieSmartphone++ });
    smartphones.push_back({ "Samsung", "Galaxy Z Fold 3", 2021, "Android",
                           "Snapdragon 888", 12, 512, "Alta", 1800, 12, 4, 12,
                           7.6, numeroSerieSmartphone++ });
    smartphones.push_back({ "Google", "Pixel 5", 2020, "Android",
                           "Snapdragon 765G", 8, 128, "Media", 700, 13, 8, 16,
                           6.0, numeroSerieSmartphone++ });
    smartphones.push_back({ "OnePlus", "9 Pro", 2021, "Android", "Snapdragon 888",
                           12, 256, "Alta", 970, 14, 16, 48, 6.7,
                           numeroSerieSmartphone++ });
    smartphones.push_back({ "Huawei", "P40 Pro", 2020, "Android", "Kirin 990", 8,
                           256, "Alta", 800, 15, 32, 50, 6.58,
                           numeroSerieSmartphone++ });
    smartphones.push_back({ "Xiaomi", "Mi 11", 2021, "Android", "Snapdragon 888",
                           12, 256, "Alta", 800, 16, 20, 108, 6.81,
                           numeroSerieSmartphone++ });
    smartphones.push_back({ "Oppo", "Find X3 Pro", 2021, "Android",
                           "Snapdragon 888", 12, 256, "Alta", 1100, 17, 32, 50,
                           6.7, numeroSerieSmartphone++ });
    smartphones.push_back({ "Sony", "Xperia 1 III", 2021, "Android",
                           "Snapdragon 888", 12, 256, "Alta", 1300, 18, 12, 12,
                           6.5, numeroSerieSmartphone++ });
    smartphones.push_back({ "LG", "Wing", 2020, "Android", "Snapdragon 765G", 8,
                           256, "Alta", 1000, 13, 32, 64, 6.8,
                           numeroSerieSmartphone++ });
    smartphones.push_back({ "Apple", "iPhone SE (3rd generation)", 2022, "iOS",
                           "A15 Bionic", 4, 64, "Media", 429, 11, 7, 12, 4.7,
                           numeroSerieSmartphone++ });
    smartphones.push_back({ "Samsung", "Galaxy A52", 2021, "Android",
                           "Snapdragon 720G", 6, 128, "Media", 400, 31, 32, 64,
                           6.5, numeroSerieSmartphone++ });
    smartphones.push_back({ "Google", "Pixel 5a", 2021, "Android",
                           "Snapdragon 765G", 6, 128, "Media", 450, 21, 8, 12,
                           6.34, numeroSerieSmartphone++ });
    smartphones.push_back({ "OnePlus", "Nord 2", 2021, "Android",
                           "Dimensity 1200-AI", 8, 128, "Media", 400, 41, 32, 50,
                           6.43, numeroSerieSmartphone++ });
    smartphones.push_back({ "Huawei", "P30 Pro", 2019, "Android", "Kirin 980", 8,
                           256, "Alta", 800, 11, 32, 40, 6.47,
                           numeroSerieSmartphone++ });
    smartphones.push_back({ "Xiaomi", "Redmi Note 10 Pro", 2021, "Android",
                           "Snapdragon 732G", 6, 128, "Media", 300, 21, 16, 108,
                           6.67, numeroSerieSmartphone++ });
    smartphones.push_back({ "Oppo", "Reno 6", 2021, "Android", "Dimensity 900", 8,
                           128, "Media", 450, 21, 32, 64, 6.4,
                           numeroSerieSmartphone++ });
    smartphones.push_back({ "Sony", "Xperia 10 III", 2021, "Android",
                           "Snapdragon 690", 6, 128, "Media", 400, 31, 8, 12, 6.0,
                           numeroSerieSmartphone++ });
    smartphones.push_back({ "LG", "Velvet", 2020, "Android", "Snapdragon 765G", 6,
                           128, "Media", 600, 41, 16, 48, 6.8,
                           numeroSerieSmartphone++ });
    smartphones.push_back({ "Apple", "iPhone 13 Pro", 2021, "iOS", "A15 Bionic", 6,
                           256, "Alta", 1000, 21, 12, 12, 6.1,
                           numeroSerieSmartphone++ });
    smartphones.push_back({ "Samsung", "Galaxy Note 20", 2020, "Android",
                           "Exynos 990", 8, 256, "Alta", 1000, 11, 10, 108, 6.7,
                           numeroSerieSmartphone++ });
    smartphones.push_back({ "Google", "Pixel 6", 2021, "Android", "Google Tensor",
                           8, 128, "Alta", 700, 11, 8, 50, 6.4,
                           numeroSerieSmartphone++ });
    smartphones.push_back({ "OnePlus", "9", 2021, "Android", "Snapdragon 888", 8,
                           128, "Alta", 730, 21, 16, 48, 6.55,
                           numeroSerieSmartphone++ });
    smartphones.push_back({ "Huawei", "Mate 40 Pro", 2020, "Android", "Kirin 9000",
                           8, 256, "Alta", 1200, 41, 13, 50, 6.76,
                           numeroSerieSmartphone++ });

    // Inicialización de laptops (15 unidades)
    int numeroSerieLaptop = 100000;
    laptops.push_back({ "Apple",
                       "MacBook Air",
                       "macOS",
                       "Apple M1",
                       8,
                       {"SSD", 256},
                       999,
                       13.3,
                       "Apple Integrated",
                       false,
                       11,
                       numeroSerieLaptop++ });
    laptops.push_back({ "Dell",
                       "XPS 13",
                       "Windows",
                       "Intel Core i7",
                       16,
                       {"SSD", 512},
                       1499,
                       13.4,
                       "Intel Iris Xe",
                       false,
                       21,
                       numeroSerieLaptop++ });
    laptops.push_back({ "HP",
                       "Spectre x360",
                       "Windows",
                       "Intel Core i7",
                       16,
                       {"SSD", 512},
                       1399,
                       13.5,
                       "Intel Iris Xe",
                       false,
                       31,
                       numeroSerieLaptop++ });
    laptops.push_back({ "Lenovo",
                       "ThinkPad X1 Carbon",
                       "Windows",
                       "Intel Core i5",
                       16,
                       {"SSD", 512},
                       1299,
                       14.0,
                       "Intel Iris Xe",
                       false,
                       31,
                       numeroSerieLaptop++ });
    laptops.push_back({ "Asus",
                       "ROG Zephyrus G14",
                       "Windows",
                       "AMD Ryzen 9",
                       16,
                       {"SSD", 1000},
                       1449,
                       14.0,
                       "NVIDIA GeForce RTX 3060",
                       true,
                       21,
                       numeroSerieLaptop++ });
    laptops.push_back({ "Microsoft",
                       "Surface Laptop 4",
                       "Windows 10",
                       "Intel Core i7",
                       16,
                       {"SSD", 512},
                       1499,
                       13.5,
                       "Intel Iris Xe",
                       false,
                       51,
                       numeroSerieLaptop++ });
    laptops.push_back({ "Razer",
                       "Blade 15",
                       "Windows",
                       "Intel Core i7",
                       16,
                       {"SSD", 1000},
                       2399,
                       15.6,
                       "NVIDIA GeForce RTX 3070 Ti",
                       true,
                       11,
                       numeroSerieLaptop++ });
    laptops.push_back({ "Samsung",
                       "Galaxy Book Pro",
                       "Windows",
                       "Intel Core i7",
                       16,
                       {"SSD", 512},
                       1199,
                       15.6,
                       "Intel Iris Xe",
                       false,
                       21,
                       numeroSerieLaptop++ });
    laptops.push_back({ "Acer",
                       "Predator Triton 300",
                       "Windows",
                       "Intel Core i7",
                       16,
                       {"SSD", 1000},
                       1799,
                       15.6,
                       "NVIDIA GeForce RTX 3070",
                       true,
                       21,
                       numeroSerieLaptop++ });
    laptops.push_back({ "Alienware",
                       "m15 R5",
                       "Windows 11",
                       "AMD Ryzen 7",
                       16,
                       {"SSD", 512},
                       2199,
                       15.6,
                       "NVIDIA GeForce RTX 3070",
                       true,
                       11,
                       numeroSerieLaptop++ });
    laptops.push_back({ "HP",
                       "Pavilion x360",
                       "Windows 11",
                       "Intel Core i5",
                       8,
                       {"SSD", 256},
                       899,
                       14.0,
                       "Intel Iris Xe",
                       false,
                       31,
                       numeroSerieLaptop++ });
    laptops.push_back({ "Apple",
                       "MacBook Pro 16",
                       "macOS",
                       "Apple M1 Max",
                       32,
                       {"SSD", 1000},
                       3599,
                       16.2,
                       "Apple Integrated",
                       false,
                       12,
                       numeroSerieLaptop++ });
    laptops.push_back({ "Lenovo",
                       "Yoga 7i",
                       "Windows 11",
                       "Intel Core i5",
                       8,
                       {"SSD", 256},
                       749,
                       14.0,
                       "Intel Iris Xe",
                       false,
                       21,
                       numeroSerieLaptop++ });
    laptops.push_back({ "Microsoft",
                       "Surface Book 3",
                       "Windows",
                       "Intel Core i7",
                       32,
                       {"SSD", 1000},
                       2999,
                       15.0,
                       "NVIDIA GeForce GTX 1660 Ti",
                       true,
                       12,
                       numeroSerieLaptop++ });
    laptops.push_back({ "Asus",
                       "ZenBook Duo 14",
                       "Windows",
                       "Intel Core i7",
                       16,
                       {"SSD", 512},
                       1299,
                       14.0,
                       "Intel Iris Xe",
                       false,
                       13,
                       numeroSerieLaptop++ });

    // Inicialización de tablets (20 unidades)
    int numeroSerieTablet = 100000;
    tablets.push_back({ "Apple", "iPad Pro", "iPadOS", "Apple M1", 8, 128, 1099,
                       12.9, 21, numeroSerieTablet++ });
    tablets.push_back({ "Samsung", "Galaxy Tab S7+", "Android", "Snapdragon 865+",
                       8, 256, 849, 12.4, 11, numeroSerieTablet++ });
    tablets.push_back({ "Microsoft", "Surface Pro 7+", "Windows", "Intel Core i7",
                       16, 1000, 1999, 12.3, 11, numeroSerieTablet++ });
    tablets.push_back({ "Amazon", "Fire HD 10", "Fire OS", "Octa-core 2.0 GHz", 3,
                       32, 149, 10.1, 21, numeroSerieTablet++ });
    tablets.push_back({ "Lenovo", "Tab P11 Pro", "Android", "Snapdragon 730G", 6,
                       128, 499, 11.5, 31, numeroSerieTablet++ });
    tablets.push_back({ "Huawei", "MatePad Pro", "HarmonyOS", "Kirin 990", 6, 128,
                       699, 10.8, 41, numeroSerieTablet++ });
    tablets.push_back({ "Samsung", "Galaxy Tab A7 Lite", "Android",
                       "MediaTek Helio P22T", 3, 32, 159, 8.7, 11,
                       numeroSerieTablet++ });
    tablets.push_back({ "Apple", "iPad mini", "iPadOS", "A15 Bionic", 4, 64, 499,
                       8.3, 21, numeroSerieTablet++ });
    tablets.push_back({ "Microsoft", "Surface Go 3", "Windows",
                       "Intel Pentium Gold", 4, 64, 399, 10.5, 31,
                       numeroSerieTablet++ });
    tablets.push_back({ "Amazon", "Fire HD 8", "Fire OS", "Quad-Core 2.0 GHz", 2,
                       32, 89, 8.0, 21, numeroSerieTablet++ });
    tablets.push_back({ "Lenovo", "Tab P11", "Android", "Snapdragon 662", 4, 64,
                       249, 11.0, 41, numeroSerieTablet++ });
    tablets.push_back({ "Huawei", "MediaPad M5 Lite", "Android", "Kirin 710", 4,
                       64, 299, 10.1, 31, numeroSerieTablet++ });
    tablets.push_back({ "Samsung", "Galaxy Tab S6 Lite", "Android", "Exynos 9611",
                       4, 64, 349, 10.4, 21, numeroSerieTablet++ });
    tablets.push_back({ "Apple", "iPad 10th Gen", "iPadOS", "A14 Bionic", 4, 64,
                       499, 10.2, 14, numeroSerieTablet++ });
    tablets.push_back({ "Microsoft", "Surface Pro 8", "Windows", "Intel Core i7",
                       16, 1000, 2799, 13.0, 16, numeroSerieTablet++ });
    tablets.push_back({ "Samsung", "Galaxy Tab S6", "Android", "Snapdragon 855", 6,
                       128, 729, 10.5, 17, numeroSerieTablet++ });
    tablets.push_back({ "Amazon", "Fire 7", "Fire OS", "Quad-Core 1.3 GHz", 1, 16,
                       49, 7.0, 19, numeroSerieTablet++ });
    tablets.push_back({ "Lenovo", "Yoga Smart Tab", "Android", "Snapdragon 439", 3,
                       64, 249, 10.1, 13, numeroSerieTablet++ });
    tablets.push_back({ "Huawei", "MatePad T10s", "Android", "Kirin 710A", 3, 32,
                       179, 10.1, 12, numeroSerieTablet++ });
    tablets.push_back({ "Apple", "iPad Pro 11", "iPadOS", "Apple M1", 8, 256, 799,
                       11.0, 15, numeroSerieTablet++ });
}

// Procedimiento para mostrar el menu principal
void mostrarMenuPrincipal() {
    limpiarPantalla();
    system("color 07");
    cout << "====== Menu Principal ======" << endl;
    cout << "1. Modulo Inventario" << endl;
    cout << "2. Modulo Administrador" << endl;
    cout << "3. Modulo Ventas" << endl;
    cout << "4. Cerrar sesion" << endl;
    cout << "5. Salir" << endl;
    cout << "Ingrese una opcion: ";
}

// Procedimiento para mostrar el menú de inventario
void mostrarMenuInventario() {
    limpiarPantalla();
    system("color 07");
    cout << "====== Menu Inventario ======" << endl;
    cout << "1. Menu de Tablets" << endl;
    cout << "2. Menu de Laptops" << endl;
    cout << "3. Menu de Smartphones" << endl;
    cout << "4. Gestionar existencias" << endl;
    cout << "5. Volver al menu principal" << endl;
    cout << "Ingrese una opcion: ";
}

// Procedimiento para mostrar el menú de tablets
void mostrarMenuTablets() {
    limpiarPantalla();
    system("color 07");
    cout << "====== Menu de Tablets ======" << endl;
    cout << "1. Mostrar tablets con RAM superior a un valor dado" << endl;
    cout << "2. Mostrar listado de productos ordenado por precio" << endl;
    cout << "3. Mostrar tablets con ROM de 64GB o mas" << endl;
    cout << "4. Mostrar tablets con sistema operativo Android" << endl;
    cout << "5. Volver al menu de inventario" << endl;
    cout << "Ingrese una opcion: ";
}

// Procedimiento para mostrar el menú de laptops
void mostrarMenuLaptops() {
    limpiarPantalla();
    cout << "====== Menu de Laptops ======" << endl;
    cout << "1. Reporte de laptops por marca" << endl;
    cout << "2. Reporte de laptops con SSD de mas de 512GB" << endl;
    cout << "3. Listado de laptops gamers en existencias" << endl;
    cout << "4. Volver al menu inventario" << endl;
    cout << "Ingrese una opcion: ";
}

// Procedimiento para mostrar el menú de smartphones
void mostrarMenuSmartphones() {
    limpiarPantalla();
    system("color 07");
    cout << "====== Menu de Smartphones ======" << endl;
    cout << "1) Reporte de smartphones con mas de 10 existencias en inventario"
        << endl;
    cout << "2) Listado de smartphones con procesador superior a un valor dado"
        << endl;
    cout << "3) Lista de smartphones ordenada por anno de fabricacion "
        "(descendente)"
        << endl;
    cout << "4) Listado de smartphones con RAM de 4GB o mas" << endl;
    cout << "5) Listado de smartphones de gama media" << endl;
    cout << "6) Volver al menu inventario" << endl;
    cout << "Ingrese una opcion: ";
}
// Procedimiento para modificar las existencias de los smartphones
void modificarExistenciasSmartphone() {
    limpiarPantalla();
    int numeroSerie;
    bool validoSerie = false, validoExistencias = false;

    while (!validoSerie) {
        cout << "Ingrese el numero de serie del smartphone: ";
        if (cin >> numeroSerie) {
            validoSerie = true;
        }
        else {
            cout << "Entrada no valida. Por favor, ingrese un numero entero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    for (int i = 0; i < smartphones.size(); ++i) {
        if (smartphones[i].numSerie == numeroSerie) {
            // Encontramos el smartphone en el inventario
            cout << "Existencias actuales: " << smartphones[i].existencias << endl;
            while (!validoExistencias) {
                cout << "Ingrese las nuevas existencias: ";
                if (cin >> smartphones[i].existencias) {
                    validoExistencias = true;
                }
                else {
                    cout << "Entrada no valida. Por favor, ingrese un numero entero." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            cout << "Las existencias se han actualizado correctamente." << endl;
            cout << "Presione cualquier tecla para continuar..." << endl;
            _getch();
            return;
        }
    }
    cout << "No se encontro ningun smartphone con el numero de serie proporcionado." << endl;
}

// Procedimiento para modificar las existencias de las laptops
void modificarExistenciasLaptop() {
    limpiarPantalla();
    int numeroSerie;
    bool validoSerie = false, validoExistencias = false;

    while (!validoSerie) {
        cout << "Ingrese el numero de serie de la laptop: ";
        if (cin >> numeroSerie) {
            validoSerie = true;
        }
        else {
            cout << "Entrada no valida. Por favor, ingrese un numero entero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    for (int i = 0; i < laptops.size(); ++i) {
        if (laptops[i].numSerie == numeroSerie) {
            // Encontramos la laptop en el inventario
            cout << "Existencias actuales: " << laptops[i].existencias << endl;
            while (!validoExistencias) {
                cout << "Ingrese las nuevas existencias: ";
                if (cin >> laptops[i].existencias) {
                    validoExistencias = true;
                }
                else {
                    cout << "Entrada no valida. Por favor, ingrese un numero entero." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            cout << "Las existencias se han actualizado correctamente." << endl;
            cout << "Presione cualquier tecla para continuar..." << endl;
            _getch();
            return;
        }
    }
    cout << "No se encontro ninguna laptop con el numero de serie proporcionado." << endl;
}

// Procedimiento para modificar las existencias de las tablets
void modificarExistenciasTablet() {
    limpiarPantalla();
    int numeroSerie;
    bool validoSerie = false, validoExistencias = false;

    while (!validoSerie) {
        cout << "Ingrese el numero de serie de la tablet: ";
        if (cin >> numeroSerie) {
            validoSerie = true;
        }
        else {
            cout << "Entrada no valida. Por favor, ingrese un numero entero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    for (int i = 0; i < tablets.size(); ++i) {
        if (tablets[i].numSerie == numeroSerie) {
            // Encontramos la tablet en el inventario
            cout << "Existencias actuales: " << tablets[i].existencias << endl;
            while (!validoExistencias) {
                cout << "Ingrese las nuevas existencias: ";
                if (cin >> tablets[i].existencias) {
                    validoExistencias = true;
                }
                else {
                    cout << "Entrada no valida. Por favor, ingrese un numero entero." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            cout << "Las existencias se han actualizado correctamente." << endl;
            cout << "Presione cualquier tecla para continuar..." << endl;
            _getch();
            return;
        }
    }
    cout << "No se encontro ninguna tablet con el numero de serie proporcionado." << endl;
}

// Procedimiento para mostrar el menú de modificación de existencias
void menuModificarExistencias() {
    limpiarPantalla();

    if (rolUltimoUsuario == "Gerente" || isAdminLoggedIn) {
        cout << "===== Modificar Existencias =====" << endl;
        cout << "1. Modificar existencias de smartphones" << endl;
        cout << "2. Modificar existencias de laptops" << endl;
        cout << "3. Modificar existencias de tablets" << endl;
        cout << "4. Regresar al menu anterior" << endl;
        cout << "Ingrese una opcion: ";
        char opcion;
        cin >> opcion;

        switch (opcion) {
        case '1':
            modificarExistenciasSmartphone();
            break;
        case '2':
            modificarExistenciasLaptop();
            break;
        case '3':
            modificarExistenciasTablet();
            break;
        case '4':
            return; // Regresar al menú anterior
        default:
            cout << "Opcion no valida. Intente nuevamente." << endl;
            cout << "Presione cualquier tecla para continuar..." << endl;
            _getch();
            break;
        }
    }
    else {
        cout << "Acceso denegado. Esta funcion solo esta disponible para los roles "
            "de Administrador y Gerente."
            << endl;
        cout << "Presione cualquier tecla para continuar..." << endl;
        _getch();
    }
}

// Procedimiento para mostrar laptops de una marca específica
void mostrarLaptopsPorMarca() {
    limpiarPantalla();

    cout << "Marcas disponibles:" << endl;

    // Obtener todas las marcas únicas
    vector<string> marcasUnicas;
    for (const auto& laptop : laptops) {
        if (find(marcasUnicas.begin(), marcasUnicas.end(), laptop.marca) == marcasUnicas.end()) {
            marcasUnicas.push_back(laptop.marca);
        }
    }

    // Mostrar las marcas disponibles
    for (int i = 0; i < marcasUnicas.size(); ++i) {
        cout << i + 1 << ". " << marcasUnicas[i] << endl;
    }

    // Pedir al usuario que elija una marca
    string opcion;
    cout << "Ingrese el numero correspondiente a la marca que desea ver: ";
    cin >> opcion;

    // Validar la entrada del usuario
    int opcionNum;
    try {
        opcionNum = stoi(opcion);
    }
    catch (const invalid_argument& e) {
        cout << "Opcion invalida. Por favor, ingrese un numero valido." << endl;
        return;
    }

    if (opcionNum < 1 || opcionNum > marcasUnicas.size()) {
        cout << "Opcion invalida. Por favor, ingrese un numero valido." << endl;
        return;
    }

    // Mostrar las laptops de la marca elegida
    cout << "Laptops de la marca " << marcasUnicas[opcionNum - 1] << ":" << endl;
    for (const auto& laptop : laptops) {
        if (laptop.marca == marcasUnicas[opcionNum - 1]) {
            cout << "Modelo: " << laptop.modelo
                << ", Sistema Operativo: " << laptop.sistemaOperativo
                << ", RAM: " << laptop.ram
                << "GB, Almacenamiento: " << laptop.discoDuro.capacidad
                << "GB, Precio: $" << laptop.precio << endl;
        }
    }

    cout << endl;
}

// Procedimiento para mostrar smartphones con mas de 10 existencias
void reporteSmartphonesMasDe10Existencias() {
    limpiarPantalla();
    cout << "Smartphones con mas de 10 existencias en inventario:" << endl;
    for (const auto& smartphone : smartphones) {
        if (smartphone.existencias > 10) {
            cout << "Marca: " << smartphone.marca << ", Modelo: " << smartphone.modelo
                << ", Existencias: " << smartphone.existencias << endl;
        }
    }
    cout << endl;
}

//Funciones para comparar por precio
bool compararPorPrecioTablet(const Tablet& a, const Tablet& b) {
    return a.precio < b.precio;
}

bool compararPorPrecioLaptop(const Laptop& a, const Laptop& b) {
    return a.precio < b.precio;
}

bool compararPorPrecioSmartphone(const Smartphone& a, const Smartphone& b) {
    return a.precio < b.precio;
}

// Procedimiento para mostrar listado de tablets ordenado por precio
void mostrarListadoTabletsOrdenadoPorPrecio() {
    limpiarPantalla();
    cout << "===================================================================="
        "================================="
        "==================== LISTADO DE TABLETS ORDENADO POR PRECIO "
        "====================="
        << endl
        << endl;

    // Crear vector temporal de tablets ordenado por precio
    vector<Tablet> tabletsOrdenadas = tablets;
    sort(tabletsOrdenadas.begin(), tabletsOrdenadas.end(),
        compararPorPrecioTablet);

    // Imprimir datos de cada tablet ordenada por precio
    for (const auto& tablet : tabletsOrdenadas) {
        cout << "Marca: " << tablet.marca << ", Modelo: " << tablet.modelo
            << ", Sistema Operativo: " << tablet.sistemaOperativo
            << ", Procesador: " << tablet.procesador << ", RAM: " << tablet.ram
            << "GB, Almacenamiento: " << tablet.rom << "GB"
            << ", Precio: $" << tablet.precio << ", Pantalla: " << tablet.pantalla
            << " pulgadas"
            << ", Existencias: " << tablet.existencias
            << ", Numero de Serie: " << tablet.numSerie << endl
            << endl;
    }
}

// Procedimiento para mostrar laptops con SSD de más de 512GB
void mostrarLaptopsConSSDMayorA() {
    limpiarPantalla();
    int tamanoSSD = 512;

    cout << "Laptops con SSD de mas de " << tamanoSSD << "GB:" << endl;
    for (const auto& laptop : laptops) {
        if (laptop.discoDuro.tipo == "SSD" &&
            laptop.discoDuro.capacidad > tamanoSSD) {
            cout << "Marca: " << laptop.marca << ", Modelo: " << laptop.modelo
                << ", RAM: " << laptop.ram << "GB, "
                << "Capacidad SSD: " << laptop.discoDuro.capacidad << "GB, Precio: $"
                << laptop.precio << endl;
        }
    }
    cout << endl;
}

// Procedimiento para mostrar listado de smartphones con procesador superior
void mostrarSmartphonesConProcesadorSuperior(
    const vector<Smartphone>& smartphones) {
    limpiarPantalla();
    system("color 07");
    cout << "Ingrese el valor del procesador: ";
    string valorProcesador;
    cin >> valorProcesador;

    cout << "Smartphones con procesador superior a " << valorProcesador << ":"
        << endl;
    bool encontrado = false;
    for (const auto& smartphone : smartphones) {
        if (smartphone.procesador > valorProcesador) {
            cout << "Marca: " << smartphone.marca << ", Modelo: " << smartphone.modelo
                << ", Procesador: " << smartphone.procesador << endl;
            encontrado = true;
        }
    }
    if (!encontrado) {
        cout << "No se encontraron smartphones con procesador superior a "
            << valorProcesador << "." << endl;
    }
    cout << endl;
}

// Lista por año de fabricacion de smartphones
bool compararPorAnioFabricacionDescendente(const Smartphone& a,
    const Smartphone& b) {
    return a.anioFabricacion > b.anioFabricacion;
}

void listaSmartphonesPorAnioFabricacionDescendente() {
    limpiarPantalla();
    cout
        << "Lista de smartphones ordenados por anno de fabricacion (descendente):"
        << endl;

    // Crear vector temporal de smartphones ordenados por año de fabricación
    vector<Smartphone> smartphonesOrdenados = smartphones;
    sort(smartphonesOrdenados.begin(), smartphonesOrdenados.end(),
        compararPorAnioFabricacionDescendente);

    // Mostrar smartphones ordenados
    for (const auto& smartphone : smartphonesOrdenados) {
        cout << "Marca: " << smartphone.marca << ", Modelo: " << smartphone.modelo
            << ", Sistema Operativo: " << smartphone.sistemaOperativo
            << ", Procesador: " << smartphone.procesador
            << ", Anno de Fabricacion: " << smartphone.anioFabricacion << endl;
    }
    cout << endl;
}

// Mostrar smartphones con mas de 4GB de RAM
void listaSmartphonesConRam4GBOMas() {
    limpiarPantalla();
    cout << "Smartphones con RAM de 4GB o mas:" << endl;
    for (const auto& smartphone : smartphones) {
        if (smartphone.ram >= 4) {
            cout << "Marca: " << smartphone.marca << ", Modelo: " << smartphone.modelo
                << ", RAM: " << smartphone.ram << "GB" << endl;
        }
    }
    cout << endl;
}

// Lista de Smartphones gama media
void listaSmartphonesGamaMedia() {
    limpiarPantalla();
    cout << "Smartphones de gama media:" << endl;
    for (const auto& smartphone : smartphones) {
        if (smartphone.gama == "Media") {
            cout << "Marca: " << smartphone.marca << ", Modelo: " << smartphone.modelo
                << ", Gama: " << smartphone.gama << endl;
        }
    }
    cout << endl;
}

// Mostrar todo el inventario
void mostrarTodoElInventario() {
    limpiarPantalla();

    // Crear un conjunto para almacenar las marcas y modelos únicos de los
    // productos
    set<string> productosUnicos;

    // Tablets
    vector<Tablet> tabletsOrdenadas = tablets;
    sort(tabletsOrdenadas.begin(), tabletsOrdenadas.end(),
        compararPorPrecioTablet);

    cout << "===================================================================="
        "======================================"
        << endl;
    cout << "                                           INVENTARIO DE PRODUCTOS  "
        "                                      "
        << endl;
    cout << "===================================================================="
        "======================================"
        << endl;
    cout << "                                             TABLETS DISPONIBLES    "
        "                                       "
        << endl;
    cout << "===================================================================="
        "======================================"
        << endl;

    for (const auto& tablet : tabletsOrdenadas) {
        // Verificar si ya se ha mostrado este producto
        string producto = tablet.marca + " " + tablet.modelo;
        if (productosUnicos.find(producto) == productosUnicos.end()) {
            cout << "Marca: " << tablet.marca << ", Modelo: " << tablet.modelo
                << endl;
            cout << "Sistema Operativo: " << tablet.sistemaOperativo
                << ", Procesador: " << tablet.procesador << endl;
            cout << "RAM: " << tablet.ram << "GB, Almacenamiento: " << tablet.rom
                << "GB" << endl;
            cout << "Precio: $" << tablet.precio << ", Pantalla: " << tablet.pantalla
                << " pulgadas" << endl;
            cout << "Existencias: " << tablet.existencias
                << ", Numero de Serie: " << tablet.numSerie << endl;
            cout << "----------------------------------------------------------------"
                "------------------------------------------"
                << endl;

            // Agregar el producto al conjunto de productos únicos
            productosUnicos.insert(producto);
        }
    }

    // Laptops
    vector<Laptop> laptopsOrdenadas = laptops;
    sort(laptopsOrdenadas.begin(), laptopsOrdenadas.end(),
        compararPorPrecioLaptop);

    cout << endl;
    cout << "===================================================================="
        "======================================"
        << endl;
    cout << "                                           LAPTOPS DISPONIBLES      "
        "                                       "
        << endl;
    cout << "===================================================================="
        "======================================"
        << endl;

    for (const auto& laptop : laptopsOrdenadas) {
        // Verificar si ya se ha mostrado este producto
        string producto = laptop.marca + " " + laptop.modelo;
        if (productosUnicos.find(producto) == productosUnicos.end()) {
            cout << "Marca: " << laptop.marca << ", Modelo: " << laptop.modelo
                << endl;
            cout << "Sistema Operativo: " << laptop.sistemaOperativo
                << ", Procesador: " << laptop.procesador << endl;
            cout << "RAM: " << laptop.ram
                << "GB, Almacenamiento: " << laptop.discoDuro.capacidad << "GB"
                << endl;
            cout << "Precio: $" << laptop.precio << ", Pantalla: " << laptop.pantalla
                << " pulgadas" << endl;
            cout << "Tarjeta de Video: " << laptop.tarjetaVideo
                << ", Es Gamer: " << (laptop.esGamer ? "Si" : "No") << endl;
            cout << "Existencias: " << laptop.existencias
                << ", Numero de Serie: " << laptop.numSerie << endl;
            cout << "----------------------------------------------------------------"
                "------------------------------------------"
                << endl;

            // Agregar el producto al conjunto de productos únicos
            productosUnicos.insert(producto);
        }
    }

    // Smartphones
    vector<Smartphone> smartphonesOrdenados = smartphones;
    sort(smartphonesOrdenados.begin(), smartphonesOrdenados.end(),
        compararPorPrecioSmartphone);

    cout << endl;
    cout << "===================================================================="
        "======================================"
        << endl;
    cout << "                                         SMARTPHONES DISPONIBLES    "
        "                                       "
        << endl;
    cout << "===================================================================="
        "======================================"
        << endl;

    for (const auto& smartphone : smartphonesOrdenados) {
        // Verificar si ya se ha mostrado este producto
        string producto = smartphone.marca + " " + smartphone.modelo;
        if (productosUnicos.find(producto) == productosUnicos.end()) {
            cout << "Marca: " << smartphone.marca << ", Modelo: " << smartphone.modelo
                << endl;
            cout << "Sistema Operativo: " << smartphone.sistemaOperativo
                << ", Procesador: " << smartphone.procesador << endl;
            cout << "RAM: " << smartphone.ram
                << "GB, Almacenamiento: " << smartphone.rom << "GB" << endl;
            cout << "Gama: " << smartphone.gama << ", Precio: $" << smartphone.precio
                << endl;
            cout << "Camara Frontal: " << smartphone.camaraFrontal
                << "MP, Camara Trasera: " << smartphone.camaraTrasera << "MP"
                << endl;
            cout << "Pantalla: " << smartphone.pantalla
                << " pulgadas, Existencias: " << smartphone.existencias << endl;
            cout << "Numero de Serie: " << smartphone.numSerie << endl;
            cout << "----------------------------------------------------------------"
                "------------------------------------------"
                << endl;

            // Agregar el producto al conjunto de productos únicos
            productosUnicos.insert(producto);
        }
    }

    cout << "Presione cualquier tecla para continuar..." << endl;
    _getch();
}

// Procedimiento para mostrar tablets con ROM de 64GB o más
void mostrarTabletsConRom64Omas() {
    limpiarPantalla();
    cout << "Tablets con ROM de 64GB o mas:" << endl;
    for (const auto& tablet : tablets) {
        if (tablet.rom >= 64) {
            cout << "Marca: " << tablet.marca << ", Modelo: " << tablet.modelo
                << ", ROM: " << tablet.rom << "GB" << endl;
        }
    }
    cout << endl;
}

// Procedimiento para mostrar tablets con sistema operativo Android
void mostrarTabletsConSistemaOperativoAndroid() {
    limpiarPantalla();
    cout << "Tablets con sistema operativo Android:" << endl;
    for (const auto& tablet : tablets) {
        if (tablet.sistemaOperativo == "Android") {
            cout << "Marca: " << tablet.marca << ", Modelo: " << tablet.modelo
                << ", Sistema Operativo: " << tablet.sistemaOperativo << endl;
        }
    }
    cout << endl;
}

// Procedimiento para mostrar listado de laptops gamers en existencias
void mostrarLaptopsGamersEnExistencias() {
    limpiarPantalla();
    cout << "Laptops Gamers en existencias:" << endl;
    for (const auto& laptop : laptops) {
        if (laptop.esGamer && laptop.existencias > 0) {
            cout << "Marca: " << laptop.marca << ", Modelo: " << laptop.modelo
                << ", RAM: " << laptop.ram << "GB, Precio: $" << laptop.precio
                << ", Existencias: " << laptop.existencias << endl;
        }
    }
    cout << endl;
}

// Procedimiento para mostrar el submenu del modulo de administrador
void mostrarMenuAdministrador() {
    limpiarPantalla();
    for (const auto& Usuario : usuarios) {
        if (Usuario.usuario == "Admin") { // Verificar si el usuario es Admin
            cout << "====== Modulo Administrador ======" << endl;
            cout << "1. Control de accesos" << endl;
            cout << "2. Modificacion de informacion del Inventario" << endl;
            cout << "3. Desbloquear usuarios" << endl;
            cout << "4. Gestionar usuarios" << endl;
            cout << "5. Volver al menu principal" << endl;
            cout << "Ingrese una opcion: ";
            return; // Salir del Procedimiento después de mostrar el menú del
            // administrador
        }
        else {
            cout << "Acceso denegado. Esta funcion solo esta disponible para el "
                "administrador, espera un segundo."
                << endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
}

// Procedimiento para mostrar el submenu del modulo de ventas
void mostrarMenuVentas() {
    limpiarPantalla();
    system("color 07");
    cout << "====== Modulo Ventas ======" << endl;
    cout << "1. Mostrar listado de productos" << endl;
    cout << "2. Crear orden" << endl;
    cout << "3. Generar factura" << endl;
    cout << "4. Mostrar todas las facturas" << endl;
    cout << "5. Volver al menu principal" << endl;
    cout << "Ingrese una opcion: ";
}

// Funcion para ordenar productos por precio
bool compararPorPrecio(const Tablet& a, const Tablet& b) {
    return a.precio < b.precio;
}

// Procedimiento para mostrar tablets con RAM superior a un valor dado
void mostrarTabletsConRamSuperior() {
    string valorRamStr;
    double valorRam;
    bool esNumero;

    do {
        esNumero = true;
        cout << "Ingrese el valor de RAM (en GB): ";
        cin >> valorRamStr;

        // Verificar si la cadena solo contiene dígitos y un punto decimal opcional
        for (char c : valorRamStr) {
            if (!isdigit(c) && c != '.') {
                esNumero = false;
                break;
            }
        }

        if (!esNumero) {
            cout << "Valor de RAM invalido. Debe ingresar un numero." << endl;
        }
    } while (!esNumero);

    try {
        valorRam = stod(valorRamStr);
    }
    catch (const out_of_range& e) {
        cout << "Valor de RAM fuera de rango. Debe ingresar un numero valido." << endl;
        return;
    }

    if (valorRam <= 0) {
        cout << "Valor de RAM invalido. Debe ser un numero positivo." << endl;
        return;
    }

    cout << "Tablets con RAM superior a " << valorRam << "GB:" << endl;
    bool tabletEncontrada = false;

    for (const auto& tablet : tablets) {
        if (tablet.ram > valorRam) {
            cout << "Marca: " << tablet.marca << ", Modelo: " << tablet.modelo << ", RAM: " << tablet.ram << "GB" << endl;
            tabletEncontrada = true;
        }
    }

    if (!tabletEncontrada) {
        cout << "No se encontraron tablets con RAM superior a " << valorRam << "GB." << endl;
    }
}

// Procedimiento para controlar accesos de usuarios
void controlDeAccesos() {
    static int vendedoresCreados = 0;
    static int gerentesCreados = 0;

    if (usuarios[0].usuario == "Admin") {
        string nuevoUsuario, nuevoNombre, nuevoRol, nuevaContrasena;
        bool nuevoBloqueado = false;

        cout << "Ingrese el nuevo usuario: ";
        cin >> nuevoUsuario;

        // Verificar si el usuario ya existe
        bool usuarioExistente = false;
        for (auto usuario : usuarios) {
            if (usuario.usuario == nuevoUsuario) {
                usuarioExistente = true;
                break;
            }
        }

        if (usuarioExistente) {
            cout << "Ya existe un usuario con ese nombre. Por favor, elija otro nombre de usuario." << endl;
            cout << "Presione cualquier tecla para continuar..." << endl;
            _getch();

            return; // Salir del Procedimiento si el usuario ya existe
        }

        cout << "Ingrese el nombre y apellido: ";
        cin.ignore();
        getline(cin, nuevoNombre);

        cout << "Ingrese el rol (Vendedor o Gerente): ";
        cin >> nuevoRol;

        if (nuevoRol == "Vendedor" && vendedoresCreados < 1) {
            cout << "Ingrese la contrasenna para el nuevo usuario: ";
            cin >> nuevaContrasena;

            usuarios.push_back(
                { nuevoUsuario, nuevoNombre, nuevoRol, nuevaContrasena, false });
            cout << "Usuario creado exitosamente." << endl;
            vendedoresCreados++;
        }
        else if (nuevoRol == "Gerente" && gerentesCreados < 1) {
            cout << "Ingrese la contrasenna para el nuevo usuario: ";
            cin >> nuevaContrasena;

            usuarios.push_back(
                { nuevoUsuario, nuevoNombre, nuevoRol, nuevaContrasena, false });
            cout << "Usuario creado exitosamente." << endl;
            gerentesCreados++;
        }
        else if (nuevoRol != "Vendedor" && nuevoRol != "Gerente") {
            cout << "Rol no valido. Solo se pueden crear usuarios Vendedor o Gerente."
                << endl;
        }
        else {
            cout << "No se pueden crear mas usuarios de ese rol." << endl;
        }

        cout << "Presione cualquier tecla para continuar..." << endl;
        _getch();
    }
    else {
        cout << "Acceso denegado. Esta funcion solo esta disponible para el "
            "administrador."
            << endl;
    }
}

// Procedimiento para modificar un smartphone en el inventario
void modificarSmartphone(int numeroSerie) {
    limpiarPantalla();
    for (int i = 0; i < smartphones.size(); ++i) {
        if (smartphones[i].numSerie == numeroSerie) {
            cout << "Ingrese los nuevos valores para el smartphone "
                << smartphones[i].marca << " " << smartphones[i].modelo << ":"
                << endl;
            cout << "Marca: ";
            getline(cin >> ws, smartphones[i].marca);
            cout << "Modelo: ";
            getline(cin >> ws, smartphones[i].modelo);
            cout << "Anno de fabricacion: ";
            while (!(cin >> smartphones[i].anioFabricacion)) {
                cout << "Error. Por favor, ingrese un ano valido." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Sistema operativo: ";
            getline(cin >> ws, smartphones[i].sistemaOperativo);
            cout << "Procesador: ";
            getline(cin >> ws, smartphones[i].procesador);
            cout << "RAM (GB): ";
            while (!(cin >> smartphones[i].ram)) {
                cout << "Error. Por favor, ingrese un valor valido para RAM." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Almacenamiento (GB): ";
            while (!(cin >> smartphones[i].rom)) {
                cout << "Error. Por favor, ingrese un valor valido para almacenamiento." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Gama: ";
            getline(cin >> ws, smartphones[i].gama);
            cout << "Precio: ";
            while (!(cin >> smartphones[i].precio)) {
                cout << "Error. Por favor, ingrese un valor valido para precio." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Existencias: ";
            while (!(cin >> smartphones[i].existencias)) {
                cout << "Error. Por favor, ingrese un valor valido para existencias." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Camara frontal (MP): ";
            while (!(cin >> smartphones[i].camaraFrontal)) {
                cout << "Error. Por favor, ingrese un valor valido para la camara frontal." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Camara trasera (MP): ";
            while (!(cin >> smartphones[i].camaraTrasera)) {
                cout << "Error. Por favor, ingrese un valor valido para la camara trasera." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Tamanno de pantalla (pulgadas): ";
            while (!(cin >> smartphones[i].pantalla)) {
                cout << "Error. Por favor, ingrese un valor valido para el tamano de pantalla." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "El smartphone se ha actualizado correctamente." << endl;
            return;
        }
    }
    cout << "No se encontro ningun smartphone con el numero de serie proporcionado." << endl;
}

// Procedimiento para modificar una laptop en el inventario
void modificarLaptop(int numeroSerie) {
    limpiarPantalla();
    for (int i = 0; i < laptops.size(); ++i) {
        if (laptops[i].numSerie == numeroSerie) {
            cout << "Ingrese los nuevos valores para la laptop " << laptops[i].marca
                << " " << laptops[i].modelo << ":" << endl;
            cout << "Marca: ";
            getline(cin >> ws, laptops[i].marca);
            cout << "Modelo: ";
            getline(cin >> ws, laptops[i].modelo);
            cout << "Sistema operativo: ";
            getline(cin >> ws, laptops[i].sistemaOperativo);
            cout << "Procesador: ";
            getline(cin >> ws, laptops[i].procesador);
            cout << "RAM (GB): ";
            while (!(cin >> laptops[i].ram)) {
                cout << "Error. Por favor, ingrese un valor valido para RAM." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Tipo de disco duro (HDD o SSD): ";
            getline(cin >> ws, laptops[i].discoDuro.tipo);
            cout << "Capacidad del disco duro (GB): ";
            while (!(cin >> laptops[i].discoDuro.capacidad)) {
                cout << "Error. Por favor, ingrese un valor valido para la capacidad del disco duro." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Precio: ";
            while (!(cin >> laptops[i].precio)) {
                cout << "Error. Por favor, ingrese un valor valido para precio." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Tamano de pantalla (pulgadas): ";
            while (!(cin >> laptops[i].pantalla)) {
                cout << "Error. Por favor, ingrese un valor valido para el tamano de pantalla." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Tarjeta de video: ";
            getline(cin >> ws, laptops[i].tarjetaVideo);
            int esGamer;
            cout << "Es gamer (0 = No, 1 = Si): ";
            while (!(cin >> esGamer) || (esGamer != 0 && esGamer != 1)) {
                cout << "Error. Por favor, ingrese 0 o 1." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            laptops[i].esGamer = (esGamer == 1);
            cout << "Existencias: ";
            while (!(cin >> laptops[i].existencias)) {
                cout << "Error. Por favor, ingrese un valor valido para existencias." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "La laptop se ha actualizado correctamente." << endl;
            return;
        }
    }
    cout << "No se encontro ninguna laptop con el numero de serie proporcionado." << endl;
}
// Procedimiento para modificar una tablet en el inventario
void modificarTablet(int numeroSerie) {
    limpiarPantalla();
    for (int i = 0; i < tablets.size(); ++i) {
        if (tablets[i].numSerie == numeroSerie) {
            cout << "Ingrese los nuevos valores para la tablet " << tablets[i].marca
                << " " << tablets[i].modelo << ":" << endl;
            cout << "Marca: ";
            getline(cin >> ws, tablets[i].marca);
            cout << "Modelo: ";
            getline(cin >> ws, tablets[i].modelo);
            cout << "Sistema operativo: ";
            getline(cin >> ws, tablets[i].sistemaOperativo);
            cout << "Procesador: ";
            getline(cin >> ws, tablets[i].procesador);
            cout << "RAM (GB): ";
            while (!(cin >> tablets[i].ram)) {
                cout << "Error. Por favor, ingrese un valor valido para RAM." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Almacenamiento (GB): ";
            while (!(cin >> tablets[i].rom)) {
                cout << "Error. Por favor, ingrese un valor valido para almacenamiento." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Precio: ";
            while (!(cin >> tablets[i].precio)) {
                cout << "Error. Por favor, ingrese un valor valido para precio." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Tamano de pantalla (pulgadas): ";
            while (!(cin >> tablets[i].pantalla)) {
                cout << "Error. Por favor, ingrese un valor valido para el tamano de pantalla." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Existencias: ";
            while (!(cin >> tablets[i].existencias)) {
                cout << "Error. Por favor, ingrese un valor valido para existencias." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "La tablet se ha actualizado correctamente." << endl;
            return;
        }
    }
    cout << "No se encontro ninguna tablet con el numero de serie proporcionado." << endl;
}
// Modificar productos existentes
void modificarProductoExistente() {
    char opcionCategoria;

    do {
        limpiarPantalla();

        cout << "Seleccione la categoria del producto que desea modificar:" << endl;
        cout << "1. Smartphone" << endl;
        cout << "2. Laptop" << endl;
        cout << "3. Tablet" << endl;
        cout << "4. Regresar al menu de modificar inventario" << endl;
        cout << "Ingrese una opcion: ";
        cin >> opcionCategoria;

        switch (opcionCategoria) {
        case '1': {
            int numSerie;
            cout << "Ingrese numero de serie: ";
            cin >> numSerie;
            modificarSmartphone(numSerie);
            break;
        }
        case '2': {
            int numSerie;
            cout << "Ingrese numero de serie: ";
            cin >> numSerie;
            modificarLaptop(numSerie);
            break;
        }
        case '3': {
            int numSerie;
            cout << "Ingrese numero de serie: ";
            cin >> numSerie;
            modificarTablet(numSerie);
            break;
        }
        case '4': {
            break;
        }
        default: {
            cout << "Opcion no valida. Por favor, seleccione una opcion valida."
                << endl;
            break;
        }
        }
    } while (opcionCategoria != '4');
}

// Procedimiento para agregar un smartphone al inventario
void agregarSmartphone() {
    limpiarPantalla();

    Smartphone nuevoSmartphone;

    cout << "Ingrese la marca del smartphone: ";
    getline(cin >> ws, nuevoSmartphone.marca);
    cout << "Ingrese el modelo del smartphone: ";
    getline(cin >> ws, nuevoSmartphone.modelo);
    cout << "Ingrese el ano de fabricacion del smartphone: ";
    while (!(cin >> nuevoSmartphone.anioFabricacion)) {
        cout << "Error. Por favor, ingrese un ano valido." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Ingrese el sistema operativo del smartphone: ";
    getline(cin >> ws, nuevoSmartphone.sistemaOperativo);
    cout << "Ingrese el procesador del smartphone: ";
    getline(cin >> ws, nuevoSmartphone.procesador);
    cout << "Ingrese la RAM del smartphone (GB): ";
    while (!(cin >> nuevoSmartphone.ram)) {
        cout << "Error. Por favor, ingrese un valor valido para RAM." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Ingrese el almacenamiento del smartphone (GB): ";
    while (!(cin >> nuevoSmartphone.rom)) {
        cout << "Error. Por favor, ingrese un valor valido para almacenamiento." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Ingrese la gama del smartphone: ";
    getline(cin >> ws, nuevoSmartphone.gama);
    cout << "Ingrese el precio del smartphone: ";
    while (!(cin >> nuevoSmartphone.precio)) {
        cout << "Error. Por favor, ingrese un valor valido para precio." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Ingrese las existencias del smartphone: ";
    while (!(cin >> nuevoSmartphone.existencias)) {
        cout << "Error. Por favor, ingrese un valor valido para existencias." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Ingrese la resolucion de la camara frontal del smartphone (MP): ";
    while (!(cin >> nuevoSmartphone.camaraFrontal)) {
        cout << "Error. Por favor, ingrese un valor valido para la camara frontal." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Ingrese la resolucion de la camara trasera del smartphone (MP): ";
    while (!(cin >> nuevoSmartphone.camaraTrasera)) {
        cout << "Error. Por favor, ingrese un valor valido para la camara trasera." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Ingrese el tamano de pantalla del smartphone (pulgadas): ";
    while (!(cin >> nuevoSmartphone.pantalla)) {
        cout << "Error. Por favor, ingrese un valor valido para el tamano de pantalla." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Asignar un numero de serie unico (simplemente incrementamos el ultimo
    // numero de serie)
    if (!smartphones.empty()) {
        nuevoSmartphone.numSerie = smartphones.back().numSerie + 1;
    }
    else {
        // Si no hay ningun smartphone en el inventario, asignamos el numero de
        // serie 100000
        nuevoSmartphone.numSerie = 100000;
    }

    // Agregar el nuevo smartphone al inventario
    smartphones.push_back(nuevoSmartphone);

    cout << "El smartphone ha sido agregado al inventario correctamente." << endl;

    cout << "Presione cualquier tecla para continuar..." << endl;
    _getch();
}


// Procedimiento para agregar una laptop al inventario
void agregarLaptop() {
    limpiarPantalla();

    Laptop nuevaLaptop;

    cout << "Ingrese la marca de la laptop: ";
    getline(cin >> ws, nuevaLaptop.marca);
    cout << "Ingrese el modelo de la laptop: ";
    getline(cin >> ws, nuevaLaptop.modelo);
    cout << "Ingrese el sistema operativo de la laptop: ";
    getline(cin >> ws, nuevaLaptop.sistemaOperativo);
    cout << "Ingrese el procesador de la laptop: ";
    getline(cin >> ws, nuevaLaptop.procesador);
    cout << "Ingrese la RAM de la laptop (GB): ";
    while (!(cin >> nuevaLaptop.ram)) {
        cout << "Error. Por favor, ingrese un valor valido para RAM." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Ingrese el tipo de disco duro de la laptop (HDD o SSD): ";
    getline(cin >> ws, nuevaLaptop.discoDuro.tipo);
    cout << "Ingrese la capacidad del disco duro de la laptop (GB): ";
    while (!(cin >> nuevaLaptop.discoDuro.capacidad)) {
        cout << "Error. Por favor, ingrese un valor valido para capacidad." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Ingrese el precio de la laptop: ";
    while (!(cin >> nuevaLaptop.precio)) {
        cout << "Error. Por favor, ingrese un valor valido para precio." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Ingrese el tamano de pantalla de la laptop (pulgadas): ";
    while (!(cin >> nuevaLaptop.pantalla)) {
        cout << "Error. Por favor, ingrese un valor valido para tamano de pantalla." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Ingrese la tarjeta de video de la laptop: ";
    getline(cin >> ws, nuevaLaptop.tarjetaVideo);
    cout << "La laptop es gamer? (0 = No, 1 = Si): ";
    while (!(cin >> nuevaLaptop.esGamer) || (nuevaLaptop.esGamer != 0 && nuevaLaptop.esGamer != 1)) {
        cout << "Error. Por favor, ingrese 0 o 1." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Ingrese las existencias de la laptop: ";
    while (!(cin >> nuevaLaptop.existencias)) {
        cout << "Error. Por favor, ingrese un valor valido para existencias." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Asignar un numero de serie unico (simplemente incrementamos el ultimo
    // numero de serie)
    if (!laptops.empty()) {
        nuevaLaptop.numSerie = laptops.back().numSerie + 1;
    }
    else {
        // Si no hay ninguna laptop en el inventario, asignamos el numero de serie
        // 100000
        nuevaLaptop.numSerie = 100000;
    }

    // Agregar la nueva laptop al inventario
    laptops.push_back(nuevaLaptop);

    cout << "La laptop ha sido agregada al inventario correctamente." << endl;

    cout << "Presione cualquier tecla para continuar..." << endl;
    _getch();
}

// Procedimiento para agregar una tablet al inventario
void agregarTablet() {
    limpiarPantalla();

    Tablet nuevaTablet;

    cout << "Ingrese la marca de la tablet: ";
    getline(cin >> ws, nuevaTablet.marca);
    cout << "Ingrese el modelo de la tablet: ";
    getline(cin >> ws, nuevaTablet.modelo);
    cout << "Ingrese el sistema operativo de la tablet: ";
    getline(cin >> ws, nuevaTablet.sistemaOperativo);
    cout << "Ingrese el procesador de la tablet: ";
    getline(cin >> ws, nuevaTablet.procesador);
    cout << "Ingrese la RAM de la tablet (GB): ";
    while (!(cin >> nuevaTablet.ram)) {
        cout << "Error. Por favor, ingrese un valor valido para RAM." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Ingrese el almacenamiento de la tablet (GB): ";
    while (!(cin >> nuevaTablet.rom)) {
        cout << "Error. Por favor, ingrese un valor valido para almacenamiento." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Ingrese el precio de la tablet: ";
    while (!(cin >> nuevaTablet.precio)) {
        cout << "Error. Por favor, ingrese un valor valido para precio." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Ingrese el tamano de pantalla de la tablet (pulgadas): ";
    while (!(cin >> nuevaTablet.pantalla)) {
        cout << "Error. Por favor, ingrese un valor valido para el tamano de pantalla." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Ingrese las existencias de la tablet: ";
    while (!(cin >> nuevaTablet.existencias)) {
        cout << "Error. Por favor, ingrese un valor valido para existencias." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Asignar un numero de serie unico (simplemente incrementamos el ultimo
    // numero de serie)
    if (!tablets.empty()) {
        nuevaTablet.numSerie = tablets.back().numSerie + 1;
    }
    else {
        // Si no hay ninguna tablet en el inventario, asignamos el numero de serie
        // 100000
        nuevaTablet.numSerie = 100000;
    }

    // Agregar la nueva tablet al inventario
    tablets.push_back(nuevaTablet);

    cout << "La tablet ha sido agregada al inventario correctamente." << endl;

    cout << "Presione cualquier tecla para continuar..." << endl;
    _getch();
}
// Conjunto para almacenar números de serie de dispositivos eliminados
set<int> eliminados;

// Procedimiento para quitar un smartphone del inventario
void quitarSmartphone() {
    limpiarPantalla();

    int numSerie;
    bool encontrado = false;

    cout << "Ingrese el numero de serie del smartphone que desea quitar: ";
    cin >> numSerie;

    // Verificar si el smartphone ya fue eliminado previamente
    if (eliminados.find(numSerie) != eliminados.end()) {
        cout << "Este smartphone ya ha sido eliminado previamente del inventario."
            << endl;
        cout << "Presione cualquier tecla para continuar..." << endl;
        _getch();
        return;
    }

    // Buscar el smartphone en el inventario
    for (auto it = smartphones.begin(); it != smartphones.end(); ++it) {
        if (it->numSerie == numSerie) {
            encontrado = true;
            cout << "Informacion del smartphone a quitar:" << endl;
            cout << "Marca: " << it->marca << ", Modelo: " << it->modelo
                << ", Precio: $" << it->precio << endl;
            cout << "Esta seguro que desea quitar este smartphone del inventario? "
                "(S/N): ";
            char confirmacion;
            cin >> confirmacion;
            if (confirmacion == 'S' || confirmacion == 's') {
                smartphones.erase(it);
                eliminados.insert(
                    numSerie); // Registrar el número de serie como eliminado
                cout << "El smartphone ha sido quitado del inventario correctamente."
                    << endl;
            }
            else {
                cout << "Operacion cancelada. El smartphone no ha sido quitado del "
                    "inventario."
                    << endl;
            }
            break;
        }
    }

    if (!encontrado) {
        cout << "No se encontro ningun smartphone con el numero de serie "
            "especificado."
            << endl;
    }

    cout << "Presione cualquier tecla para continuar..." << endl;
    _getch();
}

// Procedimiento para quitar una laptop del inventario
void quitarLaptop() {
    limpiarPantalla();

    int numSerie;
    bool encontrado = false;

    cout << "Ingrese el numero de serie de la laptop que desea quitar: ";
    cin >> numSerie;

    // Verificar si la laptop ya fue eliminada previamente
    if (eliminados.find(numSerie) != eliminados.end()) {
        cout << "Esta laptop ya ha sido eliminada previamente del inventario."
            << endl;
        cout << "Presione cualquier tecla para continuar..." << endl;
        _getch();
        return;
    }

    // Buscar la laptop en el inventario
    for (auto it = laptops.begin(); it != laptops.end(); ++it) {
        if (it->numSerie == numSerie) {
            encontrado = true;
            cout << "Informacion de la laptop a quitar:" << endl;
            cout << "Marca: " << it->marca << ", Modelo: " << it->modelo
                << ", Precio: $" << it->precio << endl;
            cout
                << "Esta seguro que desea quitar esta laptop del inventario? (S/N): ";
            char confirmacion;
            cin >> confirmacion;
            if (confirmacion == 'S' || confirmacion == 's') {
                laptops.erase(it);
                eliminados.insert(
                    numSerie); // Registrar el número de serie como eliminado
                cout << "La laptop ha sido quitada del inventario correctamente."
                    << endl;
            }
            else {
                cout << "Operacion cancelada. La laptop no ha sido quitada del "
                    "inventario."
                    << endl;
            }
            break;
        }
    }

    if (!encontrado) {
        cout << "No se encontro ninguna laptop con el numero de serie especificado."
            << endl;
    }

    cout << "Presione cualquier tecla para continuar..." << endl;
    _getch();
}

// Procedimiento para quitar una tablet del inventario
void quitarTablet() {
    limpiarPantalla();

    int numSerie;
    bool encontrado = false;

    cout << "Ingrese el numero de serie de la tablet que desea quitar: ";
    cin >> numSerie;

    // Verificar si la tablet ya fue eliminada previamente
    if (eliminados.find(numSerie) != eliminados.end()) {
        cout << "Esta tablet ya ha sido eliminada previamente del inventario."
            << endl;
        cout << "Presione cualquier tecla para continuar..." << endl;
        _getch();
        return;
    }

    // Buscar la tablet en el inventario
    for (auto it = tablets.begin(); it != tablets.end(); ++it) {
        if (it->numSerie == numSerie) {
            encontrado = true;
            cout << "Informacion de la tablet a quitar:" << endl;
            cout << "Marca: " << it->marca << ", Modelo: " << it->modelo
                << ", Precio: $" << it->precio << endl;
            cout
                << "Esta seguro que desea quitar esta tablet del inventario? (S/N): ";
            char confirmacion;
            cin >> confirmacion;
            if (confirmacion == 'S' || confirmacion == 's') {
                tablets.erase(it);
                eliminados.insert(
                    numSerie); // Registrar el número de serie como eliminado
                cout << "La tablet ha sido quitada del inventario correctamente."
                    << endl;
            }
            else {
                cout << "Operacion cancelada. La tablet no ha sido quitada del "
                    "inventario."
                    << endl;
            }
            break;
        }
    }

    if (!encontrado) {
        cout << "No se encontro ninguna tablet con el numero de serie especificado."
            << endl;
    }

    cout << "Presione cualquier tecla para continuar..." << endl;
    _getch();
}

// Procedimiento para agregar un nuevo producto al inventario
void agregarNuevoProducto() {
    char opcionCategoria;

    do {
        limpiarPantalla();

        cout << "Seleccione la categoria del producto que desea agregar:" << endl;
        cout << "1. Smartphone" << endl;
        cout << "2. Laptop" << endl;
        cout << "3. Tablet" << endl;
        cout << "4. Regresar al menu de modificar inventario" << endl;
        cout << "Ingrese una opcion: ";
        cin >> opcionCategoria;

        switch (opcionCategoria) {
        case '1': {
            agregarSmartphone();
            break;
        }
        case '2': {
            agregarLaptop();
            break;
        }
        case '3': {
            agregarTablet();
            break;
        }
        case '4': {
            break;
        }
        default: {
            cout << "Opcion no valida. Por favor, seleccione una opcion valida."
                << endl;
            break;
        }
        }
    } while (opcionCategoria != '4');
}

// Procedimiento para quitar un producto del inventario
void quitarProducto() {
    char opcionCategoria;

    do {
        limpiarPantalla();

        cout << "Seleccione la categoria del producto que desea quitar del "
            "inventario:"
            << endl;
        cout << "1. Smartphone" << endl;
        cout << "2. Laptop" << endl;
        cout << "3. Tablet" << endl;
        cout << "4. Regresar al menu de modificar inventario" << endl;
        cout << "Ingrese una opcion: ";
        cin >> opcionCategoria;

        switch (opcionCategoria) {
        case '1': {
            quitarSmartphone();
            break;
        }
        case '2': {
            quitarLaptop();
            break;
        }
        case '3': {
            quitarTablet();
            break;
        }
        case '4': {
            break;
        }
        default: {
            cout << "Opcion no valida. Por favor, seleccione una opcion valida."
                << endl;
            break;
        }
        }
    } while (opcionCategoria != '4');
}

// Procedimiento para modificar un producto en el inventario
void modificarProducto() {
    char opcionCategoria;

    do {
        limpiarPantalla();

        cout << "Seleccione la accion que desea realizar:" << endl;
        cout << "1. Agregar nuevo producto al inventario" << endl;
        cout << "2. Modificar producto existente en el inventario" << endl;
        cout << "3. Quitar producto del inventario" << endl;
        cout << "4. Regresar al menu de administrador" << endl;
        cout << "Ingrese una opcion: ";
        cin >> opcionCategoria;

        switch (opcionCategoria) {
        case '1': {
            // Agregar nuevo producto al inventario
            agregarNuevoProducto();
            break;
        }
        case '2': {
            // Modificar producto existente en el inventario
            modificarProductoExistente();
            break;
        }
        case '3': {
            // Quitar producto del inventario
            quitarProducto();
            break;
        }
        case '4': {
            break;
        }
        default: {
            cout << "Opcion no valida. Por favor, seleccione una opcion valida."
                << endl;
            break;
        }
        }
    } while (opcionCategoria != '4');
}
// Procedimiento para agregar un smartphone a la orden
void agregarSmartphoneAOrden() {
    limpiarPantalla();
    int numSerie;
    bool validoSerie = false, validoCantidad = false;

    while (!validoSerie) {
        cout << "Ingrese el numero de serie del smartphone: ";
        if (cin >> numSerie) {
            validoSerie = true;
        } else {
            cout << "Entrada no valida. Por favor, ingrese un numero entero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    bool encontrado = false;
    for (const auto& smartphone : smartphones) {
        if (smartphone.numSerie == numSerie) {
            int cantidad;
            while (!validoCantidad) {
                cout << "Ingrese la cantidad a comprar: ";
                if (cin >> cantidad) {
                    validoCantidad = true;
                } else {
                    cout << "Entrada no valida. Por favor, ingrese un numero entero." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }

            if (cantidad > smartphone.existencias) {
                cout << "No hay suficientes existencias del smartphone seleccionado." << endl;
            } else {
                OrdenDetalle detalle = {"Smartphone", smartphone.marca, smartphone.modelo, cantidad, smartphone.precio};
                orden.push_back(detalle);
                cout << "El smartphone se ha agregado a la orden correctamente." << endl;
                encontrado = true;
            }
            break;
        }
    }

    if (!encontrado) {
        cout << "No se encontro un smartphone con el numero de serie proporcionado." << endl;
    }

    cout << "Presione cualquier tecla para continuar...";
    _getch();
}

// Procedimiento para agregar una laptop a la orden
void agregarLaptopAOrden() {
    limpiarPantalla();
    int numSerie;
    bool validoSerie = false, validoCantidad = false;

    while (!validoSerie) {
        cout << "Ingrese el numero de serie de la laptop: ";
        if (cin >> numSerie) {
            validoSerie = true;
        } else {
            cout << "Entrada no valida. Por favor, ingrese un numero entero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    bool encontrado = false;
    for (const auto& laptop : laptops) {
        if (laptop.numSerie == numSerie) {
            int cantidad;
            while (!validoCantidad) {
                cout << "Ingrese la cantidad a comprar: ";
                if (cin >> cantidad) {
                    validoCantidad = true;
                } else {
                    cout << "Entrada no valida. Por favor, ingrese un numero entero." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }

            if (cantidad > laptop.existencias) {
                cout << "No hay suficientes existencias de la laptop seleccionada." << endl;
            } else {
                OrdenDetalle detalle = {"Laptop", laptop.marca, laptop.modelo, cantidad, laptop.precio};
                orden.push_back(detalle);
                cout << "La laptop se ha agregado a la orden correctamente." << endl;
                encontrado = true;
            }
            break;
        }
    }

    if (!encontrado) {
        cout << "No se encontro una laptop con el numero de serie proporcionado." << endl;
    }

    cout << "Presione cualquier tecla para continuar...";
    _getch();
}

// Procedimiento para agregar una tablet a la orden
void agregarTabletAOrden() {
    limpiarPantalla();
    int numSerie;
    bool validoSerie = false, validoCantidad = false;

    while (!validoSerie) {
        cout << "Ingrese el numero de serie de la tablet: ";
        if (cin >> numSerie) {
            validoSerie = true;
        } else {
            cout << "Entrada no valida. Por favor, ingrese un numero entero." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    bool encontrado = false;
    for (const auto& tablet : tablets) {
        if (tablet.numSerie == numSerie) {
            int cantidad;
            while (!validoCantidad) {
                cout << "Ingrese la cantidad a comprar: ";
                if (cin >> cantidad) {
                    validoCantidad = true;
                } else {
                    cout << "Entrada no valida. Por favor, ingrese un numero entero." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }

            if (cantidad > tablet.existencias) {
                cout << "No hay suficientes existencias de la tablet seleccionada." << endl;
            } else {
                OrdenDetalle detalle = {"Tablet", tablet.marca, tablet.modelo, cantidad, tablet.precio};
                orden.push_back(detalle);
                cout << "La tablet se ha agregado a la orden correctamente." << endl;
                encontrado = true;
            }
            break;
        }
    }

    if (!encontrado) {
        cout << "No se encontro una tablet con el numero de serie proporcionado." << endl;
    }

    cout << "Presione cualquier tecla para continuar...";
    _getch();
}


// Estructura para almacenar la factura
struct Factura {
    string numeroFactura;
    double totalFactura;
    string nombreCliente;
    string nitCliente;
    string tipoPago;
    int cuotas;
};


// Función para obtener la fecha actual
string obtenerFechaActual() {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    string fecha = to_string(1900 + ltm.tm_year) + '-' +
        to_string(1 + ltm.tm_mon) + '-' + to_string(ltm.tm_mday);
    return fecha;
}

// Función para generar un número de factura aleatorio
string generarNumeroFactura() {
    string numeroFactura = "FAC-";
    for (int i = 0; i < 5; ++i) {
        numeroFactura +=
            to_string(rand() % 10); // Agregar un dígito aleatorio entre 0 y 9
    }
    return numeroFactura;
}

vector<Factura> facturas;
map<string, string> clientes;

// Procedimiento para imprimir la factura
void imprimirFactura() {
    limpiarPantalla();
    system("Color f0");
    string nombreEmpresa = "SmartB, SA.";
    string direccionEmpresa = "Zona 10, Ciudad de Guatemala, Guatemala";
    string telefonoEmpresa = "+502-4567-7890";
    string correoEmpresa = "info@smartb.com";
    string sitioWebEmpresa = "www.smartb.com";
    string numeroFactura = generarNumeroFactura();
    string fechaEmision = obtenerFechaActual();

    if (orden.empty()) {
        cout << "No hay productos en la orden, primero debe generar una orden para "
            "poder imprimir la factura." << endl;
        cout << "Presione cualquier tecla para continuar...";
        _getch();
    }
    else {
        string nombreCliente;
        string nitCliente;
        int tipoCliente;

        // Bucle para solicitar información del cliente hasta que se ingrese correctamente
        do {
            cout << "Ingrese '1' si el cliente es una empresa con NIT o '2' si es consumidor final: ";
            cin >> tipoCliente;
            if (tipoCliente == 1) {
                cout << "Ingrese el NIT del cliente: ";
                cin >> nitCliente;
                if (clientes.find(nitCliente) != clientes.end()) { // Verificar si el NIT ya está en el mapa
                    nombreCliente = clientes[nitCliente]; // Si el NIT está registrado, obtener el nombre del cliente asociado
                }
                else {
                    cout << "Ingrese el nombre del cliente: ";
                    cin.ignore();
                    getline(cin, nombreCliente);
                    // Almacenar el nuevo cliente en el mapa
                    clientes[nitCliente] = nombreCliente;
                }
            }
            else if (tipoCliente == 2) {
                nombreCliente = "Consumidor Final";
                nitCliente = "CF";
            }
            else {
                cout << "Opcion invalida. Intente de nuevo." << endl;
                cout << "Presione cualquier tecla para continuar..." << endl;

                _getch();
            }
        } while (tipoCliente != 1 && tipoCliente != 2); // Repetir hasta que se ingrese una opción válida


        // Solicitar tipo de pago
        string tipoPago;
        int cuotas = 1; // Por defecto, no hay cuotas
        int opcionPago;

        // Bucle para solicitar tipo de pago hasta que se ingrese correctamente
        do {
            cout << "Seleccione el tipo de pago ('1' para al contado, '2' para VisaCuotas): ";
            cin >> opcionPago;
            switch (opcionPago) {
            case 1:
                tipoPago = "Al Contado";
                break;
            case 2:
                tipoPago = "VisaCuotas";
                // Solicitar la cantidad de cuotas hasta que se ingrese correctamente
                do {
                    cout << "Seleccione la cantidad de cuotas (3, 6, 10, 18): ";
                    cin >> cuotas;
                    if (cuotas != 3 && cuotas != 6 && cuotas != 10 && cuotas != 18) {
                        cout << "Numero de cuotas invalido. Intente de nuevo." << endl;
                        cout << "Presione cualquier tecla para continuar..." << endl;
                        _getch();
                    }
                } while (cuotas != 3 && cuotas != 6 && cuotas != 10 && cuotas != 18);
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo." << endl;
                cout << "Presione cualquier tecla para continuar..." << endl;
                _getch();
            }
        } while (opcionPago != 1 && opcionPago != 2); // Repetir hasta que se ingrese una opción válida

        double totalFactura = 0.0;
        limpiarPantalla();
        cout << "=================================================================="
            "==================================" << endl;
        cout << setw(50) << nombreEmpresa << endl;
        cout << setw(65) << direccionEmpresa << endl;
        cout << setw(50) << "Telefono: " << telefonoEmpresa << endl;
        cout << setw(50) << "Correo electronico: " << correoEmpresa << endl;
        cout << setw(50) << "Sitio web: " << sitioWebEmpresa << endl;
        cout << setw(50) << "Numero de factura: " << numeroFactura << endl;
        cout << setw(50) << fechaEmision << endl;
        cout << "=================================================================="
            "==================================" << endl
            << endl;

        cout << setw(50) << "Nombre del cliente: " << nombreCliente << endl;
        cout << setw(50) << "NIT del cliente: " << nitCliente << endl;
        cout << setw(50) << "Tipo de pago: " << tipoPago;
        if (opcionPago == 2) {
            cout << " a " << cuotas << " cuotas";
        }
        cout << endl << endl;

        cout << setw(20) << "Producto" << setw(15) << "Marca" << setw(15)
            << "Modelo" << setw(10) << "Cantidad" << setw(20) << "Precio unitario"
            << setw(15) << "Subtotal" << endl;
        cout << "------------------------------------------------------------------"
            "-----------------------------------" << endl;

        for (const auto& detalle : orden) {
            double subtotal = detalle.cantidad * detalle.precioUnitario;
            cout << setw(20) << detalle.producto << setw(15) << detalle.marca
                << setw(15) << detalle.modelo << setw(10) << detalle.cantidad
                << setw(20) << "$" << detalle.precioUnitario << setw(15) << "$"
                << subtotal << endl;
            totalFactura += subtotal;
        }
        cout << "------------------------------------------------------------------"
            "-----------------------------------" << endl;
        cout << setw(50) << "Total de la factura: $" << totalFactura << endl;

        if (opcionPago == 2) {
            double valorCuota = totalFactura / cuotas;
            cout << setw(50) << "Valor de cada cuota: $" << valorCuota << endl;
        }

        cout << "=================================================================="
            "==================================" << endl;

        // Almacenar factura
        facturas.push_back({ numeroFactura, totalFactura, nombreCliente, nitCliente, tipoPago, cuotas });

        // Restar existencias de productos en inventario
        for (auto detalle : orden) {
            double subtotal = detalle.cantidad * detalle.precioUnitario;
            for (auto& producto : tablets) {
                if (producto.modelo == detalle.modelo &&
                    producto.marca == detalle.marca) {
                    producto.existencias -= detalle.cantidad;
                    break;
                }
            }
        }

        for (auto detalle : orden) {
            double subtotal = detalle.cantidad * detalle.precioUnitario;
            for (auto& producto : laptops) {
                if (producto.modelo == detalle.modelo &&
                    producto.marca == detalle.marca) {
                    producto.existencias -= detalle.cantidad;
                    break;
                }
            }
        }

        for (auto detalle : orden) {
            double subtotal = detalle.cantidad * detalle.precioUnitario;
            for (auto& producto : smartphones) {
                if (producto.modelo == detalle.modelo) {
                    producto.existencias -= detalle.cantidad;
                    break;
                }
            }
        }

        orden.clear();

        cout << "Presione cualquier tecla para continuar..." << endl;
        _getch();
    }
}






//Procedimiento para imprimir todas las facturas
void imprimirTodasLasFacturas() {
    limpiarPantalla();
    system("color f0");
    if (facturas.empty()) {
        cout << "No hay facturas almacenadas." << endl;
    }
    else {
        cout << "Facturas almacenadas:" << endl;
        cout << "============================================================================"
            "======================================" << endl;
        cout << setw(20) << "Numero de factura" << setw(20) << "Total de la factura"
            << setw(30) << "Nombre del cliente" << setw(20) << "NIT del cliente"
            << setw(20) << "Tipo de pago" << endl;
        cout << "----------------------------------------------------------------------------"
            "--------------------------------------" << endl;
        for (const auto& factura : facturas) {
            cout << setw(20) << factura.numeroFactura << setw(20) << "$"
                << factura.totalFactura << setw(30) << factura.nombreCliente
                << setw(14) << factura.nitCliente << setw(18) << factura.tipoPago << endl;
        }
        cout << "============================================================================"
            "======================================" << endl;
    }
    cout << "Presione cualquier tecla para continuar..." << endl;
    _getch();
}
// Menu superPrincipal
void mostrarMenuSuperPrincipal() {

    limpiarPantalla();

    // Mensaje de bienvenida
    cout << "\x1b[33m**************************************************" << endl;
    cout << "\x1b[33m*                                                *" << endl;
    cout << "\x1b[33m*              Bienvenido a SmartB!              *" << endl;
    cout << "\x1b[33m*                                                *" << endl;
    cout << "\x1b[33m**************************************************\x1b[0m"
        << endl
        << endl;

    cout << "\x1b[38;2;0;191;255mPor favor, elige una opcion:\x1b[0m" << endl
        << endl;
    cout << "\x1b[97m1. Ingresar" << endl; // Opción para ingresar al sistema
    cout << "2. Salir" << endl << endl;    // Opción para salir del sistema

    // Solicitamos la entrada del usuario
    cout << "Ingresa tu opcion: ";
}

int main() {
    char opcionSuperPrincipal;

    do {
        mostrarMenuSuperPrincipal();
        cin >> opcionSuperPrincipal;

        switch (opcionSuperPrincipal) {
        case '1':
            inicializarDatos();    // Inicializar datos de productos
            inicializarUsuarios(); // Inicializar usuarios

            if (iniciarSesion()) {
                char opcion;
                bool salir = false;
                string respuesta;

                while (!salir) {

                    mostrarMenuPrincipal();
                    cin >> opcion;

                    switch (opcion) {
                    case '1': {
                        char opcionInventario;
                        do {
                            mostrarMenuInventario();
                            cin >> opcionInventario;

                            switch (opcionInventario) {
                            case '1': {
                                while (true) {
                                    mostrarMenuTablets();
                                    char opcionTablets;
                                    cin >> opcionTablets;

                                    switch (opcionTablets) {
                                    case '1':
                                        mostrarTabletsConRamSuperior();
                                        break;
                                    case '2':
                                        mostrarListadoTabletsOrdenadoPorPrecio();
                                        break;
                                    case '3':
                                        mostrarTabletsConRom64Omas();
                                        break;
                                    case '4':
                                        mostrarTabletsConSistemaOperativoAndroid();
                                        break;
                                    case '5':
                                        break;
                                    default:
                                        cout << "Opcion no valida." << endl;
                                        break;
                                    }

                                    if (opcionTablets == '5') {
                                        break;
                                    }

                                    cout << "Presione cualquier tecla para continuar...";
                                    _getch();
                                }
                                break;
                            }
                            case '2': {
                                while (true) {
                                    mostrarMenuLaptops();
                                    char opcionLaptops;
                                    cin >> opcionLaptops;

                                    switch (opcionLaptops) {
                                    case '1':
                                        mostrarLaptopsPorMarca();
                                        break;
                                    case '2':
                                        mostrarLaptopsConSSDMayorA();
                                        break;
                                    case '3':
                                        mostrarLaptopsGamersEnExistencias();
                                        break;
                                    case '4':
                                        break;
                                    default:
                                        cout << "Opcion no valida." << endl;
                                        break;
                                    }

                                    if (opcionLaptops == '4') {
                                        break;
                                    }

                                    cout << "Presione cualquier tecla para continuar...";
                                    _getch();
                                }
                                break;
                            }
                            case '3': {
                                while (true) {
                                    mostrarMenuSmartphones();
                                    char opcionSmartphones;
                                    cin >> opcionSmartphones;

                                    switch (opcionSmartphones) {
                                    case '1':

                                        reporteSmartphonesMasDe10Existencias();
                                        break;
                                    case '2':

                                        mostrarSmartphonesConProcesadorSuperior(smartphones);
                                        break;
                                    case '3':
                                        listaSmartphonesPorAnioFabricacionDescendente();
                                        break;
                                    case '4':
                                        listaSmartphonesConRam4GBOMas();
                                        break;
                                    case '5':
                                        listaSmartphonesGamaMedia();
                                        break;
                                    case '6':
                                        break;
                                    default:
                                        cout << "Opcion no valida." << endl;
                                        break;
                                    }

                                    if (opcionSmartphones == '6') {
                                        break;
                                    }

                                    cout << "Presione cualquier tecla para continuar...";
                                    _getch();
                                }
                                break;
                            }
                            case '4':
                                menuModificarExistencias();
                                break;

                            case '5':

                                break;
                            default:
                                cout << "Opcion no valida." << endl;
                                break;
                            }
                        } while (opcionInventario != '5');

                        break;
                    }
                    case '2': {
                        if (isAdminLoggedIn) {
                            char opcionAdministrador;
                            do {
                                mostrarMenuAdministrador();
                                cin >> opcionAdministrador;

                                switch (opcionAdministrador) {
                                case '1':
                                    controlDeAccesos();
                                    break;
                                case '2':
                                    inicializarDatos();
                                    cout << "Bienvenido al sistema de gestion de inventario."
                                        << endl;
                                    modificarProducto();
                                    cout << "Gracias por usar el sistema de gestion de "
                                        "inventario. Hasta luego!"
                                        << endl;
                                    break;
                                case '3':
                                    desbloquearUsuario();

                                case '4':
                                    gestionarUsuarios();
                                    break;
                                case '5':
                                    break;

                                default:
                                    cout << "Opcion no valida. Intente nuevamente." << endl;
                                    break;
                                }
                            } while (opcionAdministrador != '5');
                        }
                        else {
                            cout << "Acceso denegado. Esta funcion solo esta disponible para "
                                "el administrador."
                                << endl;
                            cout << "Presione cualquier tecla para continuar...";
                            _getch();
                        }
                        break;
                    }
                    case '3': {
                        if (rolUltimoUsuario == "Vendedor" || isAdminLoggedIn) {
                            char opcionVentas;
                            do {
                                mostrarMenuVentas();
                                cin >> opcionVentas;

                                switch (opcionVentas) {
                                case '1':
                                    mostrarTodoElInventario();
                                    break;
                                case '2':
                                    char opcionProducto;
                                    do {
                                        limpiarPantalla();
                                        cout << "1. Agregar smartphone" << endl;
                                        cout << "2. Agregar laptop" << endl;
                                        cout << "3. Agregar tablet" << endl;
                                        cout << "4. Finalizar orden" << endl;
                                        cout << "Ingrese una opcion: ";
                                        cin >> opcionProducto;

                                        switch (opcionProducto) {
                                        case '1':
                                            agregarSmartphoneAOrden();

                                            break;
                                        case '2':
                                            agregarLaptopAOrden();

                                            break;
                                        case '3':
                                            agregarTabletAOrden();

                                            break;
                                        case '4':
                                            break;
                                        default:
                                            cout << "Opcion no valida." << endl;
                                            break;
                                        }
                                    } while (opcionProducto != '4');
                                    break;
                                case '3':
                                    imprimirFactura();

                                    break;
                                case '4':
                                    imprimirTodasLasFacturas();
                                    break;
                                case '5':
                                    break;

                                default:
                                    cout << "Opcion invalida. Intente nuevamente." << endl;
                                    break;
                                }
                            } while (opcionVentas != '5');
                        }
                        else {
                            cout << "Acceso denegado, solo el Administrador y el Vendedor pueden entrar a este modulo" << endl;
                            cout << "Presione cualquier tecla para continuar..." << endl;
                            _getch();
                        }

                        break;
                    }
                    case '4':
                        isAdminLoggedIn = false;
                        iniciarSesion();
                        cout << "Acceso denegado." << endl;
                        break;
                    case '5':

                        cout << "Esta seguro de que desea salir? (s/n): ";
                        cin >> respuesta;
                        if (respuesta == "s" || respuesta == "S") {
                            salir = true;
                            limpiarPantalla();
                            system("color e2");
                            cout << "*****************************************" << endl;
                            cout << "*                                       *" << endl;
                            cout << "*           Gracias por visitar         *" << endl;
                            cout << "*                SmartB                 *" << endl;
                            cout << "*                                       *" << endl;
                            cout << "*****************************************" << endl;
                            cout << "          Esperamos verte pronto!       " << endl;
                            return 0;
                        }
                        break;

                    default:
                        cout << "Opcion no valida." << endl;
                        break;
                    }
                }

            }
            else {
                limpiarPantalla();
                system("color e2");
                cout << "*****************************************" << endl;
                cout << "*                                       *" << endl;
                cout << "*           Gracias por visitar         *" << endl;
                cout << "*                SmartB                 *" << endl;
                cout << "*                                       *" << endl;
                cout << "*****************************************" << endl;
                cout << "          Esperamos verte pronto!       " << endl;
                return 0;
            }

            break;
        case '2':
            limpiarPantalla();
            system("color e2");
            cout << "*****************************************" << endl;
            cout << "*                                       *" << endl;
            cout << "*           Gracias por visitar         *" << endl;
            cout << "*                SmartB                 *" << endl;
            cout << "*                                       *" << endl;
            cout << "*****************************************" << endl;
            cout << "          Esperamos verte pronto!       " << endl;
            return 0;
            break;

        default:
            cout << "Opcion no valida. Por favor, ingrese 1 o 2." << endl;
            cout << "Presione cualquier tecla para continuar...";
            _getch();
            limpiarPantalla(); // Limpiar la pantalla
            break;
        }

    } while (opcionSuperPrincipal != '2');

    return 0;


    //Programa hecho por Mynor Barrios, Santiago Alvarado y André Gil
}