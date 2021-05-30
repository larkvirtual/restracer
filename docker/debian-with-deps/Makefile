REGISTRY=registry.gitlab.com
REG_GROUP=docker-images10
#REGISTRY=blacknote.site:5050
#REG_GROUP=restracer
IMAGE=debian-plus-build-deps

all: login build push

login:
	@sudo docker login ${REGISTRY}

build:
	@sudo docker build -t ${REGISTRY}/${REG_GROUP}/${IMAGE}:build     -f Dockerfile.build      .
	@sudo docker build -t ${REGISTRY}/${REG_GROUP}/${IMAGE}:test-file -f Dockerfile.test-file  .
	@sudo docker build -t ${REGISTRY}/${REG_GROUP}/${IMAGE}:test-netw -f Dockerfile.test-netw  .

push:
	@sudo docker push ${REGISTRY}/${REG_GROUP}/${IMAGE}:build
	@sudo docker push ${REGISTRY}/${REG_GROUP}/${IMAGE}:test-file
	@sudo docker push ${REGISTRY}/${REG_GROUP}/${IMAGE}:test-netw

clean:
	@sudo docker rmi ${IMAGE}:build
	@sudo docker rmi ${IMAGE}:test-file
	@sudo docker rmi ${IMAGE}:test-netw
