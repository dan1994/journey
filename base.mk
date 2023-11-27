V?=0

ifeq ("${V}","0")
Q=@
vecho=@echo
NO_PRINT_DIRECTORY=--no-print-directory
else
Q=
vecho=@true
NO_PRINT_DIRECTORY=
endif

FORMAT_START=\e[
FORMAT_END=\e[0m

BLACK=30
RED=31
GREEN=32
BROWN=33
BLUE=34
PURPLE=35
CYAN=36
LIGHT_GRAY=37

NORMAL=0
BOLD=1
UNDERLINE=4
BLINKING=5
REVERSE_VIDEO=7

define log_message
	$(call log,@echo,#### $1 ####,,${CYAN},)
endef

define log_generate
	$(call log,${vecho},[Generate],$1,${BROWN},"\\t")
endef

define log_compile
	$(call log,${vecho},[Compile],$1,${BLUE},"\\t")
endef

define log_link
	$(call log,${vecho},[Link],$1,${PURPLE},"\\t\\t")
endef

define log_image
	$(call log,${vecho},[Image],$1,${GREEN},"\\t\\t")
endef

define log_run
	$(call log,${vecho},[Run],$1,${RED},"\\t\\t")
endef

define log_clean
	$(call log,${vecho},[Clean],,${BROWN},"\\t\\t")
endef

define log
	${1} "${FORMAT_START}${BOLD};${4}m${2}${FORMAT_END}${5}${3}"
endef
