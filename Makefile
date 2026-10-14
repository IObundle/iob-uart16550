# SPDX-FileCopyrightText: 2025 IObundle
#
# SPDX-License-Identifier: MIT

CORE := iob_uart16550

SIMULATOR ?= icarus
SYNTHESIZER ?= yosys
LINTER ?= spyglass
BOARD ?= iob_cyclonev_gt_dk
CSR_IF ?= iob

#
# Fill PY_PARAMS if not defined
ifeq ($(PY_PARAMS),)
ifneq ($(CSR_IF),)
PY_PARAMS:=$(PY_PARAMS):csr_if=$(CSR_IF)
endif
# Remove first char (:) from PY_PARAMS
PY_PARAMS:=$(shell echo $(PY_PARAMS) | cut -c2-)
endif # ifndef PY_PARAMS

BUILD_DIR ?= $(shell nix-shell --run "py2hwsw $(CORE) print_build_dir --py_params '$(PY_PARAMS)'")

VERSION ?=$(shell cat $(CORE).py | grep '"version":' | cut -d '"' -f 4)

ifneq ($(DEBUG),)
EXTRA_ARGS +=--debug_level $(DEBUG)
endif

setup:
	nix-shell --run "py2hwsw $(CORE) setup --no_verilog_lint --py_params '$(PY_PARAMS)' $(EXTRA_ARGS)"

pc-emul-run:
	nix-shell --run "make clean setup && make -C ../$(CORE)_V$(VERSION)/ pc-emul-run"

pc-emul-test:
	nix-shell --run "make clean setup && make -C ../$(CORE)_V$(VERSION)/ pc-emul-run"

sim-run:
	nix-shell --run "make clean setup && make -C ../$(CORE)_V$(VERSION)/ sim-run SIMULATOR=$(SIMULATOR)"

sim-test:
	nix-shell --run "make clean setup && make -C ../$(CORE)_V$(VERSION)/ sim-run SIMULATOR=icarus"

fpga-build:
	nix-shell --run "make clean setup"
	make -C ../$(CORE)_V$(VERSION)/ fpga-build BOARD=$(BOARD)

fpga-run:
	nix-shell --run "make clean setup && make -C ../$(CORE)_V$(VERSION)/ fpga-sw-build BOARD=$(BOARD)"
	make -C ../$(CORE)_V$(VERSION)/ fpga-run BOARD=$(BOARD)

fpga-test:
	make clean setup fpga-build BOARD=iob_aes_ku040_db_g

syn-build: clean
	nix-shell --run "make setup && make -C ../$(CORE)_V$(VERSION)/ syn-build SYNTHESIZER=$(SYNTHESIZER)"

lint-run: clean
	nix-shell --run "make setup && make -C ../$(CORE)_V$(VERSION)/ lint-run LINTER=$(LINTER)"

doc-build:
	nix-shell --run "make clean setup && make -C ../$(CORE)_V$(VERSION)/ doc-build"

doc-test:
	nix-shell --run "make clean setup && make -C ../$(CORE)_V$(VERSION)/ doc-test"


test-all: pc-emul-test sim-test fpga-test syn-build lint-run doc-build doc-test



# Install board server and client
board_server_install:
	make -C lib board_server_install

board_server_uninstall:
	make -C lib board_server_uninstall

board_server_status:
	systemctl status board_server

.PHONY: setup sim-test fpga-test doc-test test-all board_server_install board_server_uninstall board_server_status


clean:
	nix-shell --run "py2hwsw $(CORE) clean --build_dir '$(BUILD_DIR)'"
	@rm -rf ../*.summary ../*.rpt fusesoc_exports *.core
	@find . -name \*~ -delete

# Remove all __pycache__ folders with python bytecode
python-cache-clean:
	find . -name "*__pycache__" -exec rm -rf {} \; -prune

.PHONY: clean python-cache-clean

# Tester

tester-sim-run:
	nix-shell --run "make clean setup && make -C ../$(CORE)_V$(VERSION)/tester/ sim-run SIMULATOR=$(SIMULATOR)"

tester-fpga-run:
	nix-shell --run "make clean setup && make -C ../$(CORE)_V$(VERSION)/tester/ fpga-sw-build BOARD=$(BOARD)"
	make -C ../$(CORE)_V$(VERSION)/tester/ fpga-run BOARD=$(BOARD)

.PHONY: tester-sim-run tester-fpga-run

# 
# FuseSoC Targets
#

fusesoc-export: clean setup
	nix-shell --run "py2hwsw $(CORE) export_fusesoc --build_dir '$(BUILD_DIR)' --py_params '$(PY_PARAMS)'"

.PHONY: fusesoc-export

# Check if the target is `fusesoc-core-file` and define variables for it
ifneq ($(filter fusesoc-%,$(MAKECMDGOALS)),)

FS_REPO_NAME := $(subst _,-,$(CORE))-fs

# Get the latest commit hash from the remote repository
# NOTE: If you do not have write permissions to the IObundle repo, change the REPO_URL to your fork
REPO_URL := https://github.com/IObundle/$(FS_REPO_NAME)
$(info FuseSoC repo URL $(REPO_URL))

LATEST_FS_COMMIT = $(shell git ls-remote $(REPO_URL) HEAD | awk '{print $$1}')

# Using .tar.gz file from releases tab. Supported by fusesoc tool, but not yet supported by https://cores.fusesoc.net/
# define MULTILINE_TEXT
# provider:
#   name: url
#   url: https://github.com/IObundle/$(subst _,-,$(CORE))/releases/latest/download/$(CORE)_V$(VERSION).tar.gz
#   filetype: tar
# endef
# Alternative: Using sources from *-fs repo. Supported by fusesoc tool and https://cores.fusesoc.net/
define MULTILINE_TEXT
provider:
  name: github
  user: IObundle
  repo: $(FS_REPO_NAME)
  version: $(LATEST_FS_COMMIT)
endef
export MULTILINE_TEXT

endif

# NOTE: If you want to run this target from ghactions, you need to give it write permissions to the *-fs repo using a Personal Access Token (PAT).
# You need to generate a PAT to acces the *-fs repo:
#  - As org owner/admin of the *-fs repo, go to Settings > Developer settings > Fine-grained tokens > Generate new token.
#    - Select Repository access > Only select repositories (include both source and target repos).
#    - Grant Contents > Read & write (minimum for commits).
# Then you need add that PAT as a secret of this one (so that secrets.TARGET_REPO_PAT) becomes available.
#  - Add the PAT as a secret: Settings > Secrets and variables > Actions > New repository secret (name it e.g., TARGET_REPO_PAT).
# Finally, in the 'env' section of ci.yml, add: `PAT: ${{ secrets.TARGET_REPO_PAT }}`
#  - You can now use this url to have write permissions: `git clone https://x-access-token:${PAT}@github.com/your-org/target-repo.git`
#
# Automatically update *-fs repo with latest sources
fusesoc-update-fs-repo: fusesoc-export
	git clone $(REPO_URL) $(FS_REPO_NAME)
	# Delete all contents except .git directory
	find $(FS_REPO_NAME) -mindepth 1 -maxdepth 1 ! -name '.git' -exec rm -rf {} +
	# Copy fusesoc_exports contents to FS_REPO_NAME root
	cp -r fusesoc_exports/* $(FS_REPO_NAME)/
	# Commit and push
	export CUR_COMMIT=$(shell git rev-parse HEAD);\
	cd $(FS_REPO_NAME) && \
	git config user.name "ghactions[bot]" && \
	git config user.email "ci@iobundle.com" && \
	git add . && \
	git commit --allow-empty -m "Auto-update from main repo ($$CUR_COMMIT)" && \
	git push origin main;
	@echo "FS repo updated successfully"

.PHONY: fusesoc-core-file

# Generate standalone FuseSoC .core file that references pre-built sources from a remote source using 'provider' section.
fusesoc-core-file: fusesoc-update-fs-repo # fusesoc-export
	cp fusesoc_exports/$(CORE).core .
	# Append provider remote url to .core file
	printf "\n%s\n" "$$MULTILINE_TEXT" >> $(CORE).core
	echo "Generated independent $(CORE).core file (with 'provider' section)."

.PHONY: fusesoc-core-file

fusesoc-sign: fusesoc-core-file
	mkdir -p fusesoc_sign/lib
	cp $(CORE).core fusesoc_sign/lib
	nix-shell --run "cd fusesoc_sign;\
	fusesoc library add lib;\
	fusesoc core sign $(CORE) ~/.ssh/iob-fusesoc-sign-key\
	"

.PHONY: fusesoc-sign

fusesoc-publish: fusesoc-sign
	nix-shell --run "cd fusesoc_sign;\
	fusesoc core show $(CORE);\
	fusesoc-publish $(CORE) https://cores.fusesoc.net/\
	"

.PHONY: fusesoc-publish

# Release Artifacts

release-artifacts:
	make fusesoc-export
	tar -czf $(CORE)_V$(VERSION).tar.gz -C ./fusesoc_exports .

.PHONY: release-artifacts
