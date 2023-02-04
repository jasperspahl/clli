# Ideas

File parsing [png-parsing]

[png-parsing]: [https://www.youtube.com/watch?v=M9ZwuIv3xz8]

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
    - [ ] Inset @ position
- [x] Datensatz löschen
- [x] Anzeigen der Linkedlist Pointer
- [ ] Suche nach Datensatz
    - [ ] stringcompare selbst schreiben
        - [ ] unittests
    - [ ] Daten sortieren
        - [x] Sortierfunktion Linkliste
            - [x] Unit Tests
        - [ ] Implement UI
- [x] Daten Speichern
    - [x] On Close
    - [ ] On Demand

### Extra

- [x] Einfügen von mehreren Datensätzen
    - [x] Von Datei
    - [ ] Einlesen von txt files mit comments
    - [ ] Unit Tests
    - [ ] ~~UI Flow for adding multiple entries manually~~
- [ ] Löschen von mehreren Datensätzen
- [ ] Copy Datensatz into edit_flow
- [ ] Sortierung Quicksort
- [ ] Speichern mit Verschlüsselung
- [ ] Entfernen doppelter Datensätze
- [ ] get all data form github api
    - [x] get readme
    - [ ] get stars
    - [ ] get issues
- [ ] Add Readme Parsing
    - see [cli/cli: GitHub’s official command line tool](https://github.com/cli/cli)
    - maybe use [charmbracelet/glamour: Render markdown with style](https://github.com/charmbracelet/glamour) since it
      is also used by cli/cli
- [ ] Add the posibility to highlight hex numbers in hex mode 

## Q&A

Wie geh ich mit const arrays um die für enum to string brauche?
```c
enum Test {
    ABC,
    DEF,
}
const char * nameOfTest[] = {
    "ABC",
    "DEF"
};
```
