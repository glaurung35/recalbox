build_package:
  image: docker:latest
  variables:
    RUNNER_SCRIPT_TIMEOUT: 3h
  before_script:
    - apk update && apk add git bash
    - source scripts/release/functions.sh
  script:
    - set -euo pipefail
    - export files=$(git diff-tree --no-commit-id --name-only -r $CI_COMMIT_SHA)
    - export package_name=$(basename $files | cut -d"." -f1)
    - export RECALBOX_VERSION="${CI_COMMIT_TAG:-$(date '+%Y/%m/%d') ${CI_COMMIT_REF_SLUG} (${CI_PIPELINE_ID})}"
    - WORKDIR="$(pwd)" CCACHE="true" OUTPUTDIR="$(pwd)/output"
    - mkdir -p ${OUTPUTDIR}
    - docker build -t "recalbox-${ARCH}" .
    - |-
      docker run --rm --tmpfs /tmp:exec \
      --security-opt seccomp=unconfined \
      -v "${WORKDIR}:/work" \
      -v "${OUTPUTDIR}:/work/output" \
      -v "/recalbox-builds/dl:/share/dl" \
      -v "/recalbox-builds/ccaches/ccache-${ARCH}:/share/ccache" \
      -e "ARCH=${ARCH}" \
      -e "RECALBOX_VERSION=${RECALBOX_VERSION}" \
      -e "RECALBOX_CCACHE_ENABLED=${CCACHE}" \
      -e "PACKAGE=${package_name}" \
      "recalbox-${ARCH}" 2>&1 | tee build.log | grep '>>>' || tac build.log | grep '>>>' -m 1 -B 9999 | tac
    - echo $?
  tags:
    - build
  rules:
    - if: $CI_PIPELINE_SOURCE == "parent_pipeline"
  parallel:
    matrix:
      - ARCH:
