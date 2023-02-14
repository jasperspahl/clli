# Ideas

- Controllable like vim

## Testing UI Ideas

```
--ADD------------------------------------------------------------------------------
|  
|  Name: 
|
|
|  OK   Cancel
___________________________________________________________________________________

--ADD------------------------------------------------------------------------------
|  
|  Url: 
|
|
|  OK   Cancel
|
-----------------------------------------------------------------------------------

--ADD------------------------------------------------------------------------------
|  
|  a: Add Entry manually
|  b: Add Entry via Github
|
|  q: Cancel
| 
-----------------------------------------------------------------------------------

 0 --Search---------------------------------------------------------------------------
 1 |  -----------------------------------------------------------------------------
 2 |  | Some Value|
 3 |  -----------------------------------------------------------------------------
 4 |-----------------------------------------------------------------------------------
 5 |    Cli/Cli
 6 |    jasperspahl/dwm
 7 |    ...
```

## File parsing

a file consists of chunks of data all gets stored in Little Endian

```c
struct chunk {
    size_t title_length;
    char title[title_length];
    size_t data_length;
    char data[data_length];
    size_t url_length;
    char url[url_length];
    uint32_t stars;
    uint32_t issues
}
```

## Project Flow

base screen with selection options

## TODOS:

### Required

- [x] Einlesen der Datei
    - [x] Datei per CLI einlesen
    - [x] Datei per UI einlesen
- [x] Parsen der Datei
- [x] Datensatz hinzufügen
- [x] Datensatz löschen
- [x] Anzeigen der Linkedlist Pointer
- [x] Suche nach Datensatz
    - [x] stringcompare selbst schreiben
    - [x] Daten sortieren
        - [x] Sortierfunktion Linkliste
            - [x] Unit Tests
        - [x] Implement UI
- [x] Daten Speichern
    - [x] On Close
    - [x] On Demand

### Extra

- [x] Command mode
    - [x] Write a command parser
        - [x] Write string splitter
    - [x] Write Commands
        - [x] Open `:o <filename>` or `:open <filename>`
        - [x] Save `:w` or `:write`
        - [x] Save as `:w <filename>` or `:write <filename>`
        - [x] Save and quit `:wq`
        - [x] Quit `:q` or `:quit`
- [ ] Add the posibility to highlight hex numbers in hex mode
    - [ ] Add Color to the project
    - [x] The hex pointers to the currently selected element should be highlighted
- [ ] Datensatz an beliebiger position hinzufügen
- [x] Einfügen von mehreren Datensätzen
    - [x] Von Datei
    - [ ] Einlesen von txt files mit comments
    - [ ] Unit Tests
    - [ ] ~~UI Flow for adding multiple entries manually~~
- [ ] Löschen von mehreren Datensätzen
- [ ] Copy Datensatz into edit_flow
- [ ] Cursor on Detail page
    - [ ] automatically move the display rect of the `model->detail_text_pad`
    - [ ] enable vim style `i`/ `a` (open vim in insert mode on current position)
- [x] Mergesort
- [ ] ~~Speichern mit Verschlüsselung~~
- [ ] Entfernen doppelter Datensätze
    - [ ] Datensatz Hash compare on that delete on match
- [ ] get all data form github api
    - [x] get readme
    - [ ] get stars
    - [ ] get issues
- [ ] Add Readme Parsing
    - this will be quite difficult since ncurses doesn't support ansi escape codes
    - see [cli/cli: GitHub’s official command line tool](https://github.com/cli/cli)
    - maybe use [charmbracelet/glamour: Render markdown with style](https://github.com/charmbracelet/glamour) since it
      is also used by cli/cli
