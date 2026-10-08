# Mesures de performance

J'ai comparé plusieurs versions du moteur Monte Carlo pour voir ce qui
change réellement les performances.

## Environnement

- Mac Apple Silicon, architecture ARM64
- macOS 26.6.2
- AppleClang 17
- C++20, CMake et Ninja
- Compilation en Release avec `-O3 -DNDEBUG`

Ces résultats concernent ma machine et les paramètres utilisés ici.

## Paramètres du benchmark

Chaque appel calcule un million de trajectoires avec :

- Prix initial : 100
- Strike : 100
- Taux : 0.05
- Volatilité : 0.20
- Maturité : 1 an

Le benchmark commence par trois appels de chauffe avec les seeds 39 à 41.
Il mesure ensuite dix appels avec les seeds 42 à 51.

Les affichages sont faits après les mesures. Je compare les temps médians
pour limiter l'effet d'une exécution plus lente que les autres.

## Test de la LTO

La LTO permet au compilateur d'optimiser entre plusieurs fichiers source.
Je l'ai activée avec `CMAKE_INTERPROCEDURAL_OPTIMIZATION=ON`.

| Version | Temps médian |
|---|---:|
| Release | 19.482 ms |
| Release avec LTO | 19.656 ms |

Les prix affichés étaient identiques. Sur cette série, la LTO n'apporte
pas de gain visible.

## Comparaison avec le streaming

La version de référence utilise deux vecteurs : un pour les normales
et un pour les prix terminaux.

La version streaming calcule chaque prix et son payoff directement dans
la boucle, sans conserver ces vecteurs.

Le benchmark alterne l'ordre d'exécution des deux versions et vérifie
que leurs prix concordent avec une petite tolérance numérique.

| Version | Temps médian |
|---|---:|
| Référence avec vecteurs | 19.668 ms |
| Streaming | 38.942 ms |

Le streaming est environ deux fois plus lent dans cette comparaison.
Les prix affichés sont identiques pour les dix seeds.

Avec l'outil `sample`, l'empreinte mémoire du processus passe de 16.2 Mo
pour la référence à environ 849 Ko pour le streaming.

## Profilage et vectorisation

Le profilage montre environ 30 % des échantillons dans `exp` pour
la référence, contre 64 % pour le streaming. Ce sont des proportions
d'échantillons, pas des nombres d'appels.

Clang indique que la boucle de calcul des prix dans `model.cpp` est
vectorisée avec une largeur de 2. La boucle streaming ne l'est pas.

J'ai donc refait le benchmark avec la vectorisation automatique
désactivée :

| Version sans vectorisation | Temps médian |
|---|---:|
| Référence avec vecteurs | 19.361 ms |
| Streaming | 39.062 ms |

L'écart reste presque identique. La vectorisation n'explique donc pas
le ralentissement d'un facteur deux. La cause précise reste à déterminer.

Les mesures ont été faites dans plusieurs séries distinctes. Les petits
écarts entre séries ne suffisent pas à conclure à un gain.

## Version conservée

Je garde la version avec vecteurs comme référence, car elle est plus
rapide dans ces essais. Le streaming reste une variante expérimentale
qui utilise moins de mémoire.

## Relancer le benchmark

```bash
cmake -S . -B build/release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/release --target mc_benchmark
./build/release/mc_benchmark
```
