<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <q-page-sticky :offset="[18, 18]" position="bottom-right">
    <q-fab
      color="primary"
      direction="up"
      flat
      icon="mdi-help-circle-outline"
      id="help-button"
      square
      vertical-actions-align="right"
    >
      <q-fab-action
        color="secondary"
        icon="mdi-bash"
        label="recalbox-support.sh"
        label-position="left"
        square
      />
      <q-fab-action
        @click="() => open = true"
        color="secondary"
        icon="mdi-source-branch"
        label="Versions"
        label-position="left"
        square
      />
    </q-fab>
  </q-page-sticky>
  <q-dialog
    transition-hide="slide-down"
    transition-show="slide-up"
    v-model="open"
    class="versions-dialog"
  >
    <q-card
      class="bg-primary text-white background versions-dialog-card"
       style="width: 50vw; max-width: 80vw;"
    >
      <q-card-section>
        <VersionsList/>
      </q-card-section>
    </q-card>
  </q-dialog>
</template>

<script lang="ts" setup>
import { ref } from 'vue';
import VersionsList from 'components/global/VersionsList.vue';
import { useVersionsStore } from 'stores/versions';

useVersionsStore().fetch();

const open = ref<boolean>(false);
</script>

<style lang="sass">
.versions-dialog
  .versions-dialog-card
    border-left: 6px solid $accent

.q-page-container
  #help-button
    .q-btn--fab-mini:hover
      .q-icon:before
        color: $accent

    .gitbook
      i
        font-size: 18px
        margin-left: 3px
        margin-right: 3px
</style>
