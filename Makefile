CFLAGS += -Os

PREFIX ?= /usr/local

.SUFFIXES : .lhs
.MAIN : all
.PHONY : all

all : genfsa verify-sigmastar verify-sl verify-sp

.lhs.o :
	ghc -c -O2 -outputdir "${.OBJDIR}" \
	    -o "${.TARGET}" \
	    -ohi "${.TARGET:R}.hi" \
	    "${.IMPSRC}"

.o :
	ghc -O2 -outputdir "${.OBJDIR}" -o "${.TARGET}" "${.IMPSRC}"

.PHONY : install
install : all
	install -dm 0755 "${PREFIX}/bin"
	install -m 0755 genfsa "${PREFIX}/bin"
	install -m 0755 verify-sl "${PREFIX}/bin"
	install -m 0755 verify-sigmastar "${PREFIX}/bin"
	install -m 0755 verify-sp "${PREFIX}/bin"
	install -m 0755 "${.CURDIR}/dotify-att" "${PREFIX}/bin/"
	install -dm 0755 "${PREFIX}/share/man/man1"
	install -m 0644 "${.CURDIR}/doc/genfsa.1" "${PREFIX}/share/man/man1/"
