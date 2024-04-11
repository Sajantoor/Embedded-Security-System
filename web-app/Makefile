# Deploy this Node.js project to the public folder
PROJECT_NAME=security-web-app
DEPLOY_PATH= $(HOME)/cmpt433/public/myApps/$(PROJECT_NAME)-copy

all: copy install_client install_server

copy:
	@echo 'COPYING THE NODE.JS FILES TO $(DEPLOY_PATH)'
	@echo ''
	mkdir -p $(DEPLOY_PATH)
	chmod a+rwx $(DEPLOY_PATH)
	rsync -av --exclude='*/node_modules' * $(DEPLOY_PATH)
	@echo 'Do not edit any files in this folder; they are copied!' > $(DEPLOY_PATH)/DO_NOT_EDIT-FILES_COPIED.txt
	@echo ''
	@echo 'NOTE: On the host, in $(DEPLOY_PATH), it is best to run: npm install'
	@echo '      Or, just run the node_install target in this makefile.'

install_client:
	@echo ''
	@echo ''
	@echo 'INSTALLING REQUIRED NODE PACKAGES'
	@echo '(This may take some time)'
	@echo ''
	cd $(DEPLOY_PATH)/client && npm install

install_server:
	@echo ''
	@echo ''
	@echo 'INSTALLING REQUIRED NODE PACKAGES'
	@echo '(This may take some time)'
	@echo ''
	cd $(DEPLOY_PATH)/server && npm install

start_server:
	@echo ''
	@echo ''
	@echo 'STARTING SERVER'
	@echo ''
	cd $(DEPLOY_PATH)/server && npm start

start_client:
	@echo ''
	@echo ''
	@echo 'STARTING CLIENT'
	@echo ''
	cd $(DEPLOY_PATH)/client && serve -s build