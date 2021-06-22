# Qt SVG Scene

This tool parses an SVG file into Qt graphics objects and provides tools to lookup elements based on xml attributes in
the SVG file.

**WARNING** SVG support is extremely limited, but it is enough to render most static SVGs.

## Support and requirements

- QT 5.9.6 (min tested) - QT 5.15.2 (max tested)
- C++11 or later

## Based on

- [svgscene](https://github.com/fvacek/svgscene)
- [libshv](https://github.com/silicon-heaven/libshv/)
- [Qt SVG](https://github.com/qt/qtsvg)

## Roadmap

- Parametrize xml attributes to save
- Add default style attributes.
- Store style in struct, not hashtable. Maybe parse to enums.