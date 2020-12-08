<template>
  <q-page-sticky :offset="[92, 18]" position="bottom-right">
    <q-uploader
      :url="url"
      :label="type"
      multiple
      auto-upload
      :style="opened ? 'display: block' : 'display: none'"
      ref="uploader"
      flat
    >
      <template v-slot:header="scope">
        <div class="row no-wrap items-center q-pa-sm q-gutter-xs">
          <q-btn
            v-if="scope.queuedFiles.length > 0"
            icon="mdi-delete"
            color="secondary"
            @click="scope.removeQueuedFiles"
            round
            dense
            flat
            class="remove"
          >
            <q-tooltip>{{$t('general.upload.clearAll')}}</q-tooltip>
          </q-btn>
          <q-btn
            v-if="scope.uploadedFiles.length > 0"
            icon="mdi-check-all"
            @click="scope.removeUploadedFiles"
            round
            dense
            flat
          >
            <q-tooltip>{{$t('general.upload.removeUploaded')}}</q-tooltip>
          </q-btn>
          <q-spinner v-if="scope.isUploading" class="q-uploader__spinner" />
          <div class="col">
            <div class="q-uploader__title">{{ type }}</div>
            <div class="q-uploader__subtitle">{{ scope.uploadSizeLabel }} / {{ scope.uploadProgressLabel }}</div>
          </div>
          <q-btn
            v-if="scope.canAddFiles"
            color="secondary"
            type="a"
            icon="mdi-file-document-box-plus-outline"
            round
            dense
            flat
          >
            <q-uploader-add-trigger />
            <q-tooltip>{{$t('general.upload.pickFiles')}}</q-tooltip>
          </q-btn>

          <q-btn
            v-if="maximized"
            color="secondary"
            icon="mdi-window-minimize"
            @click="maximize"
            round
            dense
            flat
          >
            <q-tooltip>{{$t('general.upload.minimize')}}</q-tooltip>
          </q-btn>
          <q-btn
            v-if="!maximized"
            color="secondary"
            icon="mdi-window-maximize"
            @click="maximize"
            round
            dense
            flat
          >
            <q-tooltip>{{$t('general.upload.maximize')}}</q-tooltip>
          </q-btn>

          <q-btn
            color="secondary"
            icon="mdi-close"
            @click="close"
            round
            dense
            flat
          >
            <q-tooltip>{{$t('general.upload.close')}}</q-tooltip>
          </q-btn>

          <q-btn v-if="scope.isUploading" icon="mdi-delete" @click="scope.abort" round dense flat >
            <q-tooltip>{{$t('general.upload.abortUpload')}}</q-tooltip>
          </q-btn>
        </div>
      </template>

      <template v-slot:list="scope" :class="maximized ? 'maximized' : 'minimized'">
        <q-list>

          <q-item v-for="file in scope.files" :key="file.name">

            <q-item-section>
              <q-item-label class="full-width ellipsis">
                {{ file.name }}
              </q-item-label>

              <q-item-label caption>
                {{$t('general.upload.status')}} {{ file.__status }}
              </q-item-label>

              <q-item-label caption>
                {{ file.__sizeLabel }} / {{ file.__progressLabel }}
              </q-item-label>
            </q-item-section>

            <q-item-section top side>
              <q-btn
                class="gt-xs remove"
                flat
                dense
                round
                icon="mdi-delete"
                @click="scope.removeFile(file)"
                color="secondary"
              />
            </q-item-section>
          </q-item>

        </q-list>
      </template>
    </q-uploader>
  </q-page-sticky>
</template>

<script>
  import {BIOS} from 'src/store/api.routes'

  export default {
    name: 'UploadModule',
    created() {
      this.$root.$on('open-upload', this.open)
    },
    beforeDestroy () {
      this.$root.$off('open-upload', this.open)
    },
    mounted() {
      document.getElementsByClassName('q-uploader__list')[0].classList.add('maximized')
    },
    methods: {
      open(type) {
        this.$refs.uploader.pickFiles()
        this.type = type
        this.opened = true
      },
      close() {
        this.opened = false
      },
      maximize() {
        this.maximized = !this.maximized
        let maximized = document.getElementsByClassName('maximized')[0],
          minimized = document.getElementsByClassName('minimized')[0]

        if (maximized !== undefined) {
          maximized.classList.add('minimized')
          maximized.classList.remove('maximized')
        } else {
          minimized.classList.add('maximized')
          minimized.classList.remove('minimized')
        }
      }
    },
    data() {
      return {
        url: BIOS.upload,
        maximized: true,
        opened: false,
        type: 'bios'
      }
    }
  }
</script>

<style lang="sass">
  @mixin scrollbars($size, $foreground-color, $background-color: $primary)
    // For Google Chrome
    &::-webkit-scrollbar
      width:  $size
      height: $size

    &::-webkit-scrollbar-thumb
      background: $foreground-color

    &::-webkit-scrollbar-track
      background: $background-color

    // For Internet Explorer
    &
      scrollbar-face-color: $foreground-color
      scrollbar-track-color: $background-color

  .q-uploader
    width: 380px

    .q-btn
      &:hover
        i
          color: $accent

    .remove
      &:hover
        i
          color: $negative

  .q-uploader__title
    text-transform: uppercase
    color: $accent

  .q-uploader__list
    @include scrollbars(.7em, $secondary)
    max-height: 264px
    background: $primary
    transition: all .2s

    .q-item
      border-top: 1px solid $secondary

    .q-item__label.ellipsis
      color: white

    .q-item__label:not(.ellipsis)
      color: $secondary

  .maximized
    min-height: 60px
    padding: 8px
    height: 320px

  .minimized
    height: 0
    min-height: 0
    padding: 0
</style>
