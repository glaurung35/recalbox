<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <q-page class="background screenshots">
    <div class="row">
      <div
        :key="screenshot.name"
        class="col col-xs-12 col-sm-4 col-md-3 q-mb-md q-pl-sm q-pr-sm"
        v-for="screenshot in screenshots"
      >
        <div v-if="screenshot.urlImage">
          <q-card @click="openScreenshot(screenshot.urlImage)" class="screenshot" flat rounded>
            <q-card-section horizontal>
              <q-img :src="screenshot.urlImage" class="col" loading="lazy">
                <div
                  class="absolute-bottom text-white justify-between row items-start"
                  style="padding: .5em"
                >
                  <span class="self-center"><q-icon
                    name="mdi-calendar-clock"
                    style="font-size: 1.5em; margin-right: .25em;"
                  />
                  {{ screenshot.date }}</span>
                  <q-btn
                    class="float-right"
                    color="negative"
                    flat icon="mdi-delete"
                    round
                    @click.stop="openDeleteConfirm(screenshot.name)"
                  />
                </div>
              </q-img>
            </q-card-section>
          </q-card>
        </div>
        <div v-if="screenshot.urlVideo">
          <q-card @click="openVideo(screenshot.urlVideo)" class="screenshot" flat rounded>
            <q-card-section horizontal>
              <video class="col" loading="lazy">
                <source :src="screenshot.urlVideo" type="video/mp4" />
                <div
                  class="absolute-bottom text-white justify-between row items-start"
                  style="padding: .5em"
                >
                <span class="self-center"><q-icon
                  name="mdi-calendar-clock"
                  style="font-size: 1.5em; margin-right: .25em;"
                />
                {{ screenshot.date }}</span>
                  <q-btn
                    class="float-right"
                    color="negative"
                    flat icon="mdi-delete"
                    round
                    @click.stop="openDeleteVideoConfirm(screenshot.name)"
                  />
                </div>
              </video>
            </q-card-section>
          </q-card>
        </div>

      </div>
      <q-dialog v-model="modalImage.open" full-height full-width>
        <q-card>
          <q-card-section horizontal>
            <q-img :src="modalImage.imgUrl" @click="modalImage.open = false" class="opened"/>
          </q-card-section>
        </q-card>
      </q-dialog>

      <q-dialog v-model="modalVideo.open" full-height full-width>
        <q-card>
          <q-card-section horizontal style="height: 100%">
            <video
              controls
              style="height: 100%; margin: 0 auto"
              @click="modalVideo.open = false"
              class="opened"
            >
              <source :src="modalVideo.videoUrl" type="video/mp4" />
            </video>
          </q-card-section>
        </q-card>
      </q-dialog>
    </div>
  </q-page>
</template>

<script lang="ts" setup>
import { ref } from 'vue';
import { useMediaStore } from 'stores/media';
import { storeToRefs } from 'pinia';
import { useI18n } from 'vue-i18n';
import { useQuasar } from 'quasar';

const { t } = useI18n({ useScope: 'global' });

const $q = useQuasar();
const mediaStore = useMediaStore();

mediaStore.fetch();

const { screenshots } = storeToRefs(mediaStore);

const modalImage = ref<object>({
  open: false,
  imgUrl: '',
  selectedName: null,
});

function openScreenshot(url: string) {
  modalImage.value.imgUrl = url;
  modalImage.value.open = true;
}
function openDeleteConfirm(name: string) {
  modalImage.value.selectedName = name;
  $q.dialog({
    class: 'delete-dialog-card',
    dark: true,
    message: t('emulation.screenshots.dialogs.remove.text'),
    cancel: true,
    persistent: true,
    transitionHide: 'flip-up',
    transitionShow: 'flip-down',
  }).onOk(() => {
    mediaStore.delete(modalImage.value.selectedName);
    modalImage.value.selectedName = null;
  });
}

const modalVideo = ref<object>({
  open: false,
  videoUrl: '',
  selectedName: null,
});

function openVideo(url: string) {
  modalVideo.value.videoUrl = url;
  modalVideo.value.open = true;
}

function openDeleteVideoConfirm(name: string) {
  modalImage.value.selectedName = name;
  $q.dialog({
    class: 'delete-dialog-card',
    dark: true,
    message: t('emulation.screenshots.dialogs.removevideo.text'),
    cancel: true,
    persistent: true,
    transitionHide: 'flip-up',
    transitionShow: 'flip-down',
  }).onOk(() => {
    mediaStore.delete(modalImage.value.selectedName);
    modalImage.value.selectedName = null;
  });
}
</script>

<style lang="sass">
.screenshots
  padding: 16px 8px

  &:before
    content: "\F0104"

  .screenshot
    cursor: pointer

    .q-img__image
      transition: transform .2s
      -webkit-transition: -webkit-transform .2s

    &:hover
      .q-img__image
        transform: scale(1.1)
        -webkit-transform: scale(1.1)

    .absolute-bottom
      background: rgba(52, 73, 93, 0.47) !important

.delete-dialog-card
  border-left: 6px solid $accent
</style>
