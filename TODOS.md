# TODOS

## Personaje

### Vida del personaje
- Establecer la vida según el nivel y otros atributos - **DONE**
- Perder vida - **DONE**
- Ganar vida - **DONE**

### Mana del personaje
- Establecer el mana según el nivel y otros atributos - **DONE**
- Usar mana - **DONE**
- Ganar mana - **DONE**

### Nivel mágico
- Saber el mana que hay que gastar para subir de nivel - **DONE**
- Apuntar el mana gastado - **DONE**

### Experiencia
- Aumentar la experiencia al matar un enemigo o hacer una quest - **DONE**
- Reducir la experiencia al morir - **DONE**
- Bajar de nivel si la experiencia es menor a la del nivel que se encuentra - **DONE**

### Habilidades con las armas
- Aumentar el número de intentos cuando el personaje ataca - **DONE**
- Aumentar una habilidad al haber cumplido el número de intentos y poner el número de intentos a 0 - **DONE**
- Caso especial sería el de shielding - **DONE** (en defensa)

### Capacidad de carga
- Establecer la capacidad de carga del personaje dependiendo de la fuerza y la constitución - **DONE**
- Actualizar la capacidad de carga actual cuando:
  - el personaje coja objetos - **DONE**
  - deje objetos - **DONE**
  - suba de nivel - **DONE**

### Velocidad de movimiento
- Establecer la velocidad de movimiento dependiendo de la fuerza y la constitución - **DONE**
- Aumentar la velocidad de movimiento al subir de nivel - **DONE**
- Actualizar la velocidad de movimiento al coger o dejar objetos - **DONE**

### Hereditables
- **Fuerza** - Necesario para utilizar armas y para la capacidad de carga
- **Destreza** - Necesario para utilizar armas
- **Constitución** - Necesario para los puntos de vida y la rapidez con la que se recupera el personaje
- **Inteligencia** - Necesario para utilizar hechizos y objetos mágicos
- **Carisma** - Necesario para saber cómo de fácil encontrará pareja
- **Atractivo** - Necesario para saber cómo de fácil encontrará pareja

### Inventario
Habrá 11 slots de equipamiento para distintos tipos de objeto:
1. En el slot número 1 sólo se podrá equipar objetos de tipo **AMULETO** y hasta un máximo de 1 objeto.
2. En el slot número 2 sólo se podrá equipar objetos de tipo **CASCO** y hasta un máximo de 1 objeto.
3. En el slot número 3 sólo se podrá equipar objetos de tipo **CONTENEDORES PORTABLES** y hasta un máximo de 1 objeto.
4. En el slot número 4 sólo se podrá equipar objetos de tipo **ARMA** (una mano y dos manos) y hasta un máximo de 1 objeto.
5. En el slot número 5 sólo se podrá equipar objetos de tipo **ANILLO** y hasta un máximo de 1 objeto.
6. En el slot número 6 sólo se podrá equipar objetos de tipo **ARMADURA** y hasta un máximo de 1 objeto.
7. En el slot número 7 sólo se podrá equipar objetos de tipo **ESCUDO**, hasta un máximo de 1 objeto, siempre y cuando no esté equipada un arma de dos manos.
8. En el slot número 8 sólo se podrá equipar objetos de tipo **ANILLO** y hasta un máximo de 1 objeto.
9. En el slot número 9 sólo se podrá equipar objetos de tipo **LEGS ARMOR** y hasta un máximo de 1 objeto.
10. En el slot número 10 sólo se podrá equipar objetos de tipo **BOTAS**, hasta un máximo de 1 objeto.
11. En el slot número 11 sólo se podrá equipar objetos de tipo **MUNICION** y hasta un máximo de 100 objetos.

**Notas:**
- Cuando se equipa un arma de dos manos, el escudo pasa automáticamente al contenedor portable que esté equipado. En caso de no estar equipado un contenedor portable, el objeto cae al suelo.
- Cuando se equipa un escudo y hay un arma de dos manos equipada, el arma de dos manos pasa automáticamente al contenedor portable que esté equipado. En caso de no estar equipado un contenedor portable, el objeto cae al suelo.

#### Funciones del inventario
- Añadir objetos al inventario - **DONE**
- Quitar objetos del inventario - **DONE**
- Calcular el peso de todos los objetos del inventario - **DONE** (muy simple, necesita mejora)
- Usar un objeto del inventario - **DONE** (hay que repasarlo)

---

## Objetos
(Todos heredan de los objetos, incluidos las armas y los contenedores)

### Categoría de los objetos - **DONE**
Los objetos tendrían que tener una determinada categoría a elegir entre las siguientes:
- **Normales**, sin propiedades mágicas - Color: blanco
- **Mágicos** - Color: amarillo
- **Raros** - Color: verde
- **Épicos** - Color: azul
- **Legendarios** - Color: morado

### Sets
Los objetos de un set desbloquearán habilidades al tener el set completo.

---

## Contenedores

---

## Armas
