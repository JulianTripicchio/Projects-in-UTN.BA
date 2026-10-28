# Sweet Medical 🏥 — Plataforma de gestión de turnos médicos

Aplicación web para reservar y administrar turnos médicos, con acceso por rol (**paciente** y **médico**). Los pacientes pueden buscar profesionales y especialidades, reservar turnos y ver sus turnos activos; los médicos gestionan su agenda, disponibilidad, servicios e historial de pacientes.

🔗 **Demo online:** [sweet-medical-4.netlify.app](https://sweet-medical-4.netlify.app/)

## Stack

| Parte      | Tecnologías                                                                 |
| ---------- | --------------------------------------------------------------------------- |
| Frontend   | Next.js 16 (App Router), React 19, Ant Design, CSS propio                   |
| Backend    | Node.js, Express 5, MongoDB + Mongoose, JWT, bcrypt, Zod, node-cron         |
| Testing    | Jest + Supertest (backend), Cypress (end-to-end)                            |
| Calidad    | ESLint, Prettier                                                            |

## Estructura

```
.
├── cypress/                # Tests end-to-end
├── packages/
│   ├── backend/            # API REST (Express + MongoDB)
│   │   ├── index.js        # Punto de entrada: conecta a Mongo, inicia los cron jobs y el servidor
│   │   └── src/
│   │       ├── controllers/ services/ repositories/ models/ domain/
│   │       ├── routes/     # Rutas de la API
│   │       ├── jobs/       # Generación automática de turnos y recordatorios
│   │       ├── docs/       # Especificación OpenAPI (swaggerSpec.json)
│   │       └── tests/      # Tests unitarios y de integración
│   └── frontend/           # App Next.js
│       └── src/app/        # Páginas: login, registro, paciente/, medico/, turnos, ...
├── cypress.config.js
├── eslint.config.mjs
└── package.json            # Herramientas compartidas (lint, prettier, cypress)
```

---

## Requisitos

- **Node.js 20.9 o superior** (lo exige Next.js 16). Se recomienda Node 22 LTS.
- **pnpm**: `npm install -g pnpm` (o `corepack enable`).
- **MongoDB**: una instancia local, en Docker o en [MongoDB Atlas](https://www.mongodb.com/atlas) (tiene un plan gratuito).

### Levantar MongoDB

Elegí una opción:

```bash
# Opción 1: Docker
docker run -d --name sweet-medical-mongo -p 27017:27017 mongo:7

# Opción 2: instalación local
# Descargar MongoDB Community Server desde https://www.mongodb.com/try/download/community
# y dejar corriendo el servicio en el puerto 27017.

# Opción 3: Atlas
# Crear un cluster gratuito y copiar el connection string (mongodb+srv://...).
```

---

## Instalación

Cada paquete se instala por separado:

```bash
# 1. Herramientas de la raíz (ESLint, Prettier, Cypress)
pnpm install

# 2. Backend
cd packages/backend
pnpm install

# 3. Frontend
cd ../frontend
pnpm install
```

## Variables de entorno

### Backend — `packages/backend/.env`

```env
MONGO_URI=mongodb://localhost:27017/sweet-medical
SERVER_PORT=3001
JWT_SECRET=cambiame-por-un-secreto-largo
FRONTEND_URL=http://localhost:3000
```

| Variable       | Descripción                                                                                       |
| -------------- | ------------------------------------------------------------------------------------------------- |
| `MONGO_URI`    | Connection string de MongoDB (local o Atlas).                                                     |
| `SERVER_PORT`  | Puerto del backend. El frontend espera `3001` por defecto.                                        |
| `JWT_SECRET`   | Clave para firmar los tokens de login. **Obligatoria**: sin ella el login falla.                  |
| `FRONTEND_URL` | Orígenes permitidos por CORS, separados por coma. Si no se define, se acepta cualquier origen.    |

### Frontend — `packages/frontend/.env.local` (opcional)

```env
NEXT_PUBLIC_API_URL=http://localhost:3001
```

Si no se define, el frontend usa `http://localhost:3001`.

---

## Ejecución

Necesitás **dos terminales**.

**Terminal 1 — Backend**

```bash
cd packages/backend
pnpm dev        # con reinicio automático (node --watch)
# o
pnpm start      # sin watch
```

Deberías ver:

```
¡Conectado a MongoDB con éxito!
Backend escuchando en puerto 3001
```

Podés probar que responde en [http://localhost:3001/health](http://localhost:3001/health) → `{"status":"ok"}`.

**Terminal 2 — Frontend**

```bash
cd packages/frontend
pnpm dev
```

Abrí [http://localhost:3000](http://localhost:3000). Para empezar, creá un usuario desde **Registro** y después iniciá sesión.

### Build de producción del frontend

```bash
cd packages/frontend
pnpm build
pnpm start
```

---

## Tests

### Unitarios e integración (backend)

```bash
cd packages/backend
pnpm test
```

### End-to-end (Cypress)

Con el **backend y el frontend corriendo**, desde la raíz:

```bash
pnpm e2e              # modo headless
npx cypress open      # modo interactivo
```

## Lint y formato

Desde la raíz:

```bash
pnpm lint
pnpm format
```

---

## API

Base URL: `http://localhost:3001`

| Ruta               | Recurso                         |
| ------------------ | ------------------------------- |
| `/health`          | Healthcheck                     |
| `/auth/login`      | Login (devuelve un JWT)         |
| `/paciente`        | Alta de pacientes (registro), sus turnos |
| `/medico`          | Alta de médicos (registro), agenda y disponibilidad |
| `/turnos`          | Turnos                          |
| `/especialidades`  | Especialidades                  |
| `/sede`            | Sedes                           |
| `/obraSocial`      | Obras sociales                  |
| `/notificacion`    | Notificaciones                  |

La especificación completa está en [`packages/backend/src/docs/swaggerSpec.json`](packages/backend/src/docs/swaggerSpec.json). Se puede abrir pegándola en [editor.swagger.io](https://editor.swagger.io/) o importándola en Postman.

Al iniciar, el backend también arranca dos **cron jobs**: uno que genera turnos a partir de la disponibilidad de los médicos y otro que crea notificaciones de recordatorio de turnos.

---

## Problemas comunes

| Problema                                        | Solución                                                                                   |
| ----------------------------------------------- | ------------------------------------------------------------------------------------------ |
| `MongooseServerSelectionError` / no conecta     | MongoDB no está corriendo o `MONGO_URI` es incorrecta. En Atlas, habilitá tu IP en *Network Access*. |
| `secretOrPrivateKey must have a value` al login | Falta `JWT_SECRET` en `packages/backend/.env`.                                              |
| Error de CORS en el navegador                   | Agregá la URL del frontend a `FRONTEND_URL`.                                                |
| El frontend no trae datos                       | Verificá que el backend esté en el puerto `3001` o configurá `NEXT_PUBLIC_API_URL`.          |
| `pnpm start:backend` desde la raíz no encuentra el paquete | Los scripts de la raíz usan `pnpm --filter`, pero el workspace de la raíz no declara `packages/*`. Corré cada paquete desde su carpeta como se indica arriba. |
