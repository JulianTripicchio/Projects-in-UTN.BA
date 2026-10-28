# Development Exercises — Desarrollo de Software

Ejercicios de clase y una app integradora de la materia **Desarrollo de Software (UTN FRBA)**. Recorren desde una API REST mínima con Express hasta una arquitectura en capas y una app mobile con Expo.

| Carpeta                  | Qué es                                                                 | Tecnologías                        |
| ------------------------ | ---------------------------------------------------------------------- | ---------------------------------- |
| `Ejercicio Clase 11-4`   | Primera API REST de productos, todo en un archivo                      | Express 5, Zod                     |
| `Ejercicio Clase 18-4`   | API de productos separada en capas (rutas, controller, service, repo)  | Express 5, Zod, dotenv             |
| `Ejercicio Clase 25-04`  | **Megasuper**: CRUD de productos con descuentos y manejo de errores    | Express 5, Zod, CORS, dotenv       |
| `Ejercicio Clase 16-5`   | **Car Zero**: landing page responsive de una concesionaria             | HTML, CSS                          |
| `Ejercicio Clase 23-05`  | Base de un cliente React                                               | React 19, Create React App         |
| `Orders and Gigs App`    | **AI Do It**: app de freelancers (gigs, pedidos, chat, opiniones)      | Express 5, Zod, Expo, React Native |

## Requisitos generales

- **Node.js 22 LTS** (mínimo 20.19). `Orders and Gigs App/frontend` pide Node ≥ 22.11.
- **npm** (viene con Node). Si preferís **pnpm**, también funciona en los proyectos que tienen `pnpm-lock.yaml`.
- Para probar las APIs: [Postman](https://www.postman.com/), [Bruno](https://www.usebruno.com/), Thunder Client o `curl`.

> Todos los datos se guardan **en memoria**: al reiniciar un servidor se pierde lo cargado.

---

## Ejercicio Clase 11-4 — API REST básica

```bash
cd "Ejercicio Clase 11-4"
npm install
npm start          # o: npm run dev  (reinicia al guardar)
```

Servidor en `http://localhost:3000`.

| Método   | Ruta                 | Descripción                |
| -------- | -------------------- | -------------------------- |
| `GET`    | `/v1/healthcheck`    | Healthcheck                |
| `GET`    | `/v2/healthcheck`    | Healthcheck (v2)           |
| `GET`    | `/v1/productos`      | Lista productos            |
| `GET`    | `/v1/productos/:id`  | Obtiene un producto        |
| `POST`   | `/v1/productos`      | Crea un producto (validado con Zod) |
| `DELETE` | `/v1/productos/:id`  | Elimina un producto        |

```bash
curl http://localhost:3000/v1/healthcheck
```

---

## Ejercicio Clase 18-4 — API en capas

```bash
cd "Ejercicio Clase 18-4"
npm install
npm start          # o: npm run dev
```

Servidor en `http://localhost:3000` (se puede cambiar con la variable `PORT` en un archivo `.env`).

| Método | Ruta         | Descripción        |
| ------ | ------------ | ------------------ |
| `GET`  | `/productos` | Lista productos    |
| `POST` | `/productos` | Crea un producto   |

---

## Ejercicio Clase 25-04 — Megasuper

```bash
cd "Ejercicio Clase 25-04"
npm install
npm start          # o: npm run dev
```

Servidor en `http://localhost:3000` (configurable con `PORT` en un archivo `.env` en esta carpeta).

| Método   | Ruta              | Descripción                          |
| -------- | ----------------- | ------------------------------------ |
| `POST`   | `/productos/seed` | Carga productos de ejemplo           |
| `GET`    | `/productos`      | Lista productos                      |
| `POST`   | `/productos`      | Crea un producto                     |
| `GET`    | `/productos/:id`  | Obtiene un producto                  |
| `PUT`    | `/productos/:id`  | Actualiza un producto                |
| `DELETE` | `/productos/:id`  | Elimina un producto                  |

Tip: empezá con `POST /productos/seed` para tener datos.

**Colección de Postman:** importá [`postman/Producto-CRUD.postman_collection.json`](Ejercicio%20Clase%2025-04/postman/Producto-CRUD.postman_collection.json) en Postman (*Import → archivo*) para tener todas las requests armadas.

---

## Ejercicio Clase 16-5 — Car Zero (HTML + CSS)

No necesita instalación. Abrí `Ejercicio Clase 16-5/index.html` con doble clic en el navegador.

Si querés servirlo con un servidor local (por ejemplo, para probarlo desde el celular en la misma red):

```bash
cd "Ejercicio Clase 16-5"
npx serve .
```

o usá la extensión **Live Server** de VS Code (*clic derecho en `index.html` → Open with Live Server*).

---

## Ejercicio Clase 23-05 — Cliente React

```bash
cd "Ejercicio Clase 23-05/client"
npm install
npm start
```

Se abre en `http://localhost:3000`.

> [!NOTE]
> Es el esqueleto de la clase: `src/App.js` tiene el `return ( )` vacío, por lo que **no compila tal cual**. Para levantarlo, completá el `return` con algún JSX, por ejemplo `return <h1>Hola</h1>;`.

---

## Orders and Gigs App — AI Do It

Marketplace de servicios freelance: los freelancers publican **gigs** (servicios con paquetes) y los clientes hacen **pedidos**, chatean dentro del pedido, cambian su estado y dejan opiniones.

```
Orders and Gigs App/
├── backend/    # API REST Express (datos en memoria)
└── frontend/   # App Expo / React Native (web, Android, iOS)
```

### 1. Backend

Creá `backend/.env`:

```env
SERVER_PORT=3000
# Opcional: orígenes permitidos para CORS, separados por coma
# ALLOWED_ORIGINS=http://localhost:8081
```

> [!IMPORTANT]
> `SERVER_PORT` es obligatorio: sin él, Express levanta en un puerto aleatorio y el frontend no lo encuentra. El frontend espera el backend en el puerto **3000**.

```bash
cd "Orders and Gigs App/backend"
npm install
npm run dev        # con nodemon
# o
npm start
```

| Recurso       | Rutas principales                                                                 |
| ------------- | --------------------------------------------------------------------------------- |
| `/cliente`    | `POST /` registro · `POST /login`                                                 |
| `/freelancer` | `POST /` registro · `POST /login`                                                 |
| `/gigs`       | `GET /` buscar · `POST /` crear · `GET /:id` · `GET /categoria/:categoriaId`      |
| `/pedidos`    | `GET /` · `POST /` · `GET /:id` · `GET /cliente/:clienteId` · `GET /gig/:gigId`   |
|               | `GET/POST /:id/mensajes` · `POST /:id/opiniones` · `PATCH /:id/cancelar` · `PATCH /:id/estado` |

### 2. Frontend

En otra terminal:

```bash
cd "Orders and Gigs App/frontend"
npm install
npm run web        # abre la app en el navegador con ancho de celular
```

Otras formas de correrlo:

| Comando           | Dónde corre                                                        |
| ----------------- | ------------------------------------------------------------------ |
| `npm run web`     | Navegador                                                          |
| `npm start`       | Muestra un QR para abrir la app en **Expo Go** desde el celular    |
| `npm run android` | Emulador de Android (requiere Android Studio)                      |
| `npm run ios`     | Simulador de iOS (solo macOS con Xcode)                            |

#### Correr en el celular con Expo Go

1. Instalá **Expo Go** desde la Play Store / App Store.
2. El celular y la PC tienen que estar en la **misma red Wi-Fi**.
3. Buscá la IP de tu PC (`ipconfig` en Windows, `ip a` en Linux, `ipconfig getifaddr en0` en macOS).
4. Creá `frontend/.env` con esa IP:

   ```env
   EXPO_PUBLIC_API_BASE_URL=http://192.168.0.25:3000
   ```

5. Corré `npm start` y escaneá el QR con Expo Go (en iOS, con la cámara).

Si no se define `EXPO_PUBLIC_API_BASE_URL`, la app usa `http://localhost:3000` en web/iOS y `http://10.0.2.2:3000` en el emulador de Android.

> No reinicies el backend durante una demo: los usuarios, gigs, pedidos y mensajes se pierden. Al arrancar, registrá un cliente y/o un freelancer desde la app.

---

## Problemas comunes

| Problema                                               | Solución                                                                         |
| ------------------------------------------------------ | -------------------------------------------------------------------------------- |
| `SyntaxError: Cannot use import statement outside a module` | Actualizá Node a 22 LTS (versiones viejas no detectan módulos ES automáticamente). |
| `EADDRINUSE: address already in use :::3000`           | Ya hay otro proyecto en el puerto 3000. Cerralo o cambiá el puerto (`PORT` / `SERVER_PORT`). |
| La app de Expo no conecta con el backend desde el celular | Revisá que `EXPO_PUBLIC_API_BASE_URL` tenga la IP de la PC (no `localhost`) y que el firewall permita el puerto 3000. |
