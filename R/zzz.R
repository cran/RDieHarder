## $Date: 2011-03-24 08:36:03 -0500 (Thu, 24 Mar 2011) $
## $Id: zzz.R 47 2011-03-24 13:36:03Z dirk.eddelbuettel $

# grab the namespace
.NAMESPACE <- environment()

.onLoad <- function(lib, pkg) {
    library.dynam("RDieHarder", pkg, lib )

    ## assign lists of generators and tests to hidden variables in namespace
    assign(".dieharder.generators", dieharderGenerators(), .NAMESPACE)
    assign(".dieharder.tests", dieharderTests(), .NAMESPACE)
}

