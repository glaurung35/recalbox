<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div class="screenshots row">
    <div
      :key="screenshot.name"
      class="col col-xs-12 col-sm-4 col-md-3 q-mb-md q-pl-sm q-pr-sm"
      v-for="screenshot in screenshots"
    >
      <q-card @click="openScreenshot(screenshot.url)" class="screenshot" flat rounded>
        <q-card-section horizontal>
          <q-img :src="screenshot.url" class="col" loading="lazy">
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
    <q-dialog v-model="modal.open" full-height full-width>
      <q-card>
        <q-card-section horizontal>
          <q-img :src="modal.imgUrl" @click="modal.open = false" class="opened"/>
        </q-card-section>
      </q-card>
    </q-dialog>
  </div>
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

const modal = ref<object>({
  open: false,
  imgUrl: '',
  selectedName: null,
});

function openScreenshot(url:string) {
  modal.value.imgUrl = url;
  modal.value.open = true;
}
function openDeleteConfirm(name:string) {
  modal.value.selectedName = name;
  $q.dialog({
    class: 'delete-dialog-card',
    dark: true,
    message: t('emulation.screenshots.dialogs.remove.text'),
    cancel: true,
    persistent: true,
    transitionHide: 'flip-up',
    transitionShow: 'flip-down',
  }).onOk(() => {
    mediaStore.delete(modal.value.selectedName);
    modal.value.selectedName = null;
  });
}
</script>

<style lang="sass">
.screenshots
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
