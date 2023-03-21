<template>
  <div class="row">
    <q-dialog @click="modal.open = false" v-model="modal.open">
      <q-img :src="modal.imgUrl"/>
    </q-dialog>

    <div class="col col-xs-12 col-sm-12 col-md-12 q-mb-md q-pl-sm q-pr-sm">
      <q-btn
        :label="$t('emulation.screenshots.screenshotButton')"
        class="float-right"
        color="accent"
        icon-right="mdi-camera-enhance"
        outline
      />
    </div>
    <div
      :key="screenshot.name"
      class="col col-xs-12 col-sm-4 col-md-3 q-mb-md q-pl-sm q-pr-sm"
      v-for="screenshot in screenshots"
    >
      <q-card @click="open(screenshot.url)" class="screenshot" flat square>
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
<!--              <q-btn-->
<!--                class="float-right"-->
<!--                color="negative"-->
<!--                flat icon="mdi-delete"-->
<!--                round-->
<!--                @click.stop="openConfirm('slug')"-->
<!--              />-->
            </div>
          </q-img>
        </q-card-section>
      </q-card>

    </div>
  </div>
</template>

<script lang="ts" setup>
import { ref } from 'vue';
import { useMediaStore } from 'stores/media';
import { storeToRefs } from 'pinia';

const mediaStore = useMediaStore();
mediaStore.fetch();

const { screenshots } = storeToRefs(mediaStore);

// const screenshots: Array<object> = [
//   {
//     name: '31497_9e4635d5d3ac855e02c3f0d546c20557.png',
//     url: 'https://www.gamerz.be/data/attachment-files/2017/01/31497_9e4635d5d3ac855e02c3f0d546c20557.png',
//     date: date.formatDate(date.extractDate('2020-03-28', 'YYYY-MM-DD'), 'DD/MM/YYYY'),
//   },
//   {
//     name: 'RecalBox.png',
//     url: 'https://wiki.labomedia.org/images/a/ae/RecalBox.png',
//     date: date.formatDate(date.extractDate('2020-03-28', 'YYYY-MM-DD'), 'DD/MM/YYYY'),
//   },
//   {
//     name: '1265.png',
//     url: 'https://cdn.inpact-hardware.com/data-prod/image/bd/1265.png',
//     date: date.formatDate(date.extractDate('2020-03-28', 'YYYY-MM-DD'), 'DD/MM/YYYY'),
//   },
//   {
//     name: '1261.png',
//     url: 'https://cdn.inpact-hardware.com/data-prod/image/bd/1261.png',
//     date: date.formatDate(date.extractDate('2020-03-28', 'YYYY-MM-DD'), 'DD/MM/YYYY'),
//   },
//   {
//     name: '1266.png',
//     url: 'https://cdn.inpact-hardware.com/data-prod/image/bd/1266.png',
//     date: date.formatDate(date.extractDate('2020-04-16', 'YYYY-MM-DD'), 'DD/MM/YYYY'),
//   },
// ];

const modal = ref<object>({
  open: false,
  imgUrl: '',
});

function open(url:string) {
  modal.value.imgUrl = url;
  modal.value.open = true;
}
</script>

<style lang="sass">
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

.q-dialog__inner
  cursor: pointer

  > div
    max-width: 150vh
</style>
