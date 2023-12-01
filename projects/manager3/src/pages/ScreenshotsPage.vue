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
        <div v-if="screenshot.type === 'image'">
          <q-card @click="open('image', screenshot.path)" class="screenshot" flat rounded>
            <q-card-section horizontal>
              <q-img :src="screenshot.path" class="col" loading="lazy">
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
        <div v-if="screenshot.type === 'video'">
          <q-card @click="open('video', screenshot.path)" class="screenshot" flat rounded>
            <q-card-section horizontal>
              <video class="col" loading="lazy">
                <source :src="screenshot.path" type="video/mp4" />
              </video>
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
            </q-card-section>
          </q-card>
        </div>

      </div>
      <q-dialog v-model="openImage" full-height full-width>
        <q-card>
          <q-card-section horizontal>
            <q-img :src="mediaPath" @click="openImage = false" class="opened"/>
          </q-card-section>
        </q-card>
      </q-dialog>

      <q-dialog v-model="openVideo" full-height full-width>
        <q-card>
          <q-card-section horizontal style="height: 100%">
            <video
              controls
              style="height: 100%; margin: 0 auto"
              @click="openVideo = false"
              class="opened"
            >
              <source :src="mediaPath" type="video/mp4" />
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

const openImage = ref(false);
const openVideo = ref(false);
const mediaPath = ref('');

function open(type: string, url: string) {
  if (type === 'image') {
    mediaPath.value = url;
    openImage.value = true;
  } else if (type === 'video') {
    mediaPath.value = url;
    openVideo.value = true;
  }
}

function openDeleteConfirm(name: string) {
  $q.dialog({
    class: 'delete-dialog-card',
    dark: true,
    message: t('emulation.screenshots.dialogs.remove.text'),
    cancel: true,
    persistent: true,
    transitionHide: 'flip-up',
    transitionShow: 'flip-down',
  }).onOk(() => {
    mediaStore.delete(name);
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
