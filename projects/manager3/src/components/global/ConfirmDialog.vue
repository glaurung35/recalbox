<template>
  <q-dialog
    v-model="open"
    transition-show="flip-down"
    transition-hide="flip-up"
    persistent
  >
    <q-card class="bg-primary text-white">
      <q-card-section class="row items-center">
        <q-avatar size="50px" font-size="50px" :icon="icon" color="primary" text-color="white" />
        <span class="q-ml-sm">{{ message }}</span>
      </q-card-section>

      <q-card-actions align="right">
        <q-btn flat :label="$t('general.confirmDialog.cancel')" color="white" v-close-popup />
        <q-btn
          flat
          :label="$t('general.confirmDialog.valid')"
          color="white"
          v-close-popup
          @click="confirm"
        />
      </q-card-actions>
    </q-card>
  </q-dialog>
</template>

<script>
  export default {
    name: 'ConfirmDialog',
    created() {
      this.$root.$on('confirm-open', this.onOpen)
    },
    beforeDestroy () {
      this.$root.$off('confirm-open', this.onOpen)
    },
    methods: {
      onOpen: function(callback, icon, message){
        this.open = true
        this.callback = callback
        if (icon !== null) this.icon = icon
        if (message !== null) this.message = message
      },
      confirm: function () {
        this.callback()
      }
    },
    data() {
      return {
        open: false,
        icon: 'mdi-alert-box-outline',
        message: this.$t('general.confirmDialog.defaultMessage'),
        callback: null
      }
    },
  }
</script>

<style lang="sass">

</style>
