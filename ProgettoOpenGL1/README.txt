================================================================================
DONKEY KONG 2D- PROGETTO MIDTERM DI COMPUTER GRAPHICS
================================================================================

Applicazione grafica interattiva 2D ispirata al classico arcade Donkey Kong,
sviluppata in C++ e OpenGL 3.3 Core con GLSL, GLFW, GLAD, GLM e Dear ImGui.


--------------------------------------------------------------------------------
1. REQUISITI E DIPENDENZE
--------------------------------------------------------------------------------
- Compilatore: Visual Studio (MSVC) con supporto C++17 (o superiore)
- API Grafica: OpenGL 3.3 Core Profile
- Librerie utilizzate:
    * GLFW       (gestione finestra, contesto OpenGL e input da tastiera)
    * GLAD       (caricamento delle funzioni OpenGL)
    * GLM        (vettori e matrici 4x4 in coordinate omogenee)
    * Dear ImGui (overlay HUD per lo stato della partita)


--------------------------------------------------------------------------------
2. COMPILAZIONE E AVVIO (VISUAL STUDIO)
--------------------------------------------------------------------------------
1. Estrarre l'archivio del progetto in una cartella locale.
2. Aprire il file di soluzione (.sln) con Visual Studio.
3. Selezionare la configurazione di build desiderata (Debug o Release)
   e la piattaforma x64.
4. Compilare la soluzione tramite:
     Compila -> Compila soluzione  (Ctrl + Shift + B)
5. Avviare l'applicazione tramite:
     Debug -> Avvia senza eseguire debug  (Ctrl + F5)
   oppure:
     Debug -> Avvia debug  (F5)

NOTA SULLA WORKING DIRECTORY:
L'eseguibile carica a runtime gli shader dalla cartella "shaders/" e le mesh
OBJ/MTL dalla cartella "assets/". Se si avvia l'eseguibile direttamente fuori
da Visual Studio, assicurarsi che la directory di lavoro corrente contenga le
cartelle "assets/" e "shaders/" (oppure copiarle accanto al file .exe).


--------------------------------------------------------------------------------
3. CONTROLLI
--------------------------------------------------------------------------------
  A / D  oppure  Freccia Sinistra / Destra : Muove Mario a sinistra / destra
  W / S  oppure  Freccia Su / Giu'         : Sale / scende le scale
  Spazio                                   : Salto
  F                                        : Spara verso Donkey Kong
                                             (sulla trave superiore, dopo aver
                                             raccolto tutti e 5 i proiettili)
  R                                        : Riavvia la partita
  P                                        : [Debug] Porta le vite a 1 per far
                                             comparire subito il martello
  ESC                                      : Chiude l'applicazione


--------------------------------------------------------------------------------
4. REGOLE DI GIOCO
--------------------------------------------------------------------------------
- Raccolta: Mario inizia con 3 vite e 40 secondi per raccogliere i 5 proiettili
  sparsi sul livello, evitando i barili rotolanti lanciati da Donkey Kong.
- Scontro finale: Raccolti tutti e 5 i proiettili, il timer si ferma e Mario
  puo' salire sulla trave di Donkey Kong e premere F per sparargli (servono
  5 colpi per eliminarlo).
- Martello (Power-up): Quando resta 1 sola vita compare un martello su una
  trave intermedia. Raccogliendolo, Mario puo' distruggere i barili rotolanti
  per 5 secondi.
- Vittoria: Eliminare Donkey Kong e raggiungere Pauline sulla piattaforma
  in cima.


--------------------------------------------------------------------------------
5. STRUTTURA DEL PROGETTO
--------------------------------------------------------------------------------
  assets/
    - barrel.obj / .mtl        : Mesh 2D (curve di Hermite) dei barili statici
    - hammer.obj / .mtl        : Mesh 2D (curve di Hermite) del martello

  shaders/
    - background.vert / .frag  : Shader procedurale dedicato allo sfondo
    - basic.vert / .frag       : Shader standard per gli oggetti della scena
    - bullet.vert              : Vertex shader dedicato all'oscillazione dei
                                 5 proiettili raccoglibili

  Sorgenti C++ (.h / .cpp):
    - Geometrie e Scena        : MeshData, ShapeGenerator, ObjLoader, Geometry,
                                 Scene, Rig, TransformConfig
    - Rendering e Shader       : Shader, Renderer, BackgroundRenderer,
                                 BulletRenderer, WindowUtils, Hud
    - Logica di gioco          : Level, Mario, DonkeyKong, Pauline, Barrels,
                                 Bullets, Shots, Hammer, Collision, Game
    - Viste grafiche           : MarioView, DonkeyKongView, PaulineView,
                                 BarrelsView, BulletsView, ShotsView
    - Input e Configurazione   : Input, Controls, Config, HudInfo
