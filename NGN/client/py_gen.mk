
parser.y: cli.bnf bnf_parser.py parser.y.footer parser.y.header
	@echo [PY_GEN] $@
	$(Q) python bnf_parser.py -i $< -p parser.y

lookup.c: cli.bnf bnf_parser.py lookup.c.footer lookup.c.header
	@echo [PY_GEN] $@
	$(Q) python bnf_parser.py -i $< -l lookup.c

#ahelp.c: cli.bnf bnf_parser.py
#	@echo [PY_GEN] $@
#	$(Q) python bnf_parser.py -i $< -a ahelp.c
#
