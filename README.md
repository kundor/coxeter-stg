These programs work on [Coxeter diagrams](https://en.wikipedia.org/wiki/Coxeter%E2%80%93Dynkin_diagram) with ringed nodes, representing a polytope built by [Wythoff's construction](https://en.wikipedia.org/wiki/Wythoff_construction).
The class `FaceOrbitPoset`, constructed from a Coxeter diagram,
is a partially ordered set consisting of all the face orbits of the polytope.
The function `makeOrbit` constructs a symmetry type graph from the Face Orbit Poset.
The symmetry type graph, also known as an orbit graph, represents all the flag orbits
with edges labeled i where i-adjacent flags are in the corresponding orbits.

Coxeter diagrams are represented as graphs using the [Boost Graph Library](http://www.boost.org/doc/libs/release/libs/graph/doc/index.html).
Each vertex has properties `ringed` (boolean, initially false),
and integers`x_coord` and `y_coord` (for use in drawing the diagram.)
Each edge has an unsigned integer property `order`,
representing the order of the product of the two reflections the edge joins.
Traditionally, the edge is omitted if this order is 2 (when the mirrors are orthogonal),
and the label is omitted if this order is 3.
It is possible for the product to have infinite order, in which case
the `order` property of the edge is set to 0.

Orbit graphs are also represented using the Boost Graph Library.
Vertices have no properties, and edges have an integer property `rank`.

The `TeXout` class allows output of these Coxeter diagrams, orbit graphs,
and face orbit posets as [LaTeX](https://www.latex-project.org/),
utilizing [Ti*k*Z](https://www.ctan.org/pkg/pgf).
Simply feed these objects to a TeXout instance with `<<` operators,
similar to iostream. TeXout also collects necessary packages, Ti*k*Z libraries,
and preamble definitions, to construct the preamble when the document
is committed to a file.
After adding all your TeX, write the TeXout instance to an fstream.

[Luatex](http://www.luatex.org/) is necessary to lay out the orbit graphs.
It is included in most major TeX distributions.
When producing pdf output, the programs call `lualatex`, which should be
in your PATH.
