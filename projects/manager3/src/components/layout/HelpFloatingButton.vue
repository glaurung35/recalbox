<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <q-fab
    color="white"
    direction="up"
    flat
    icon="mdi-help-circle-outline"
    id="help-button"
    square
    vertical-actions-align="right"
    @update:model-value="update"
    :model-value="model"
    :offset="[18, 18]"
  >
    <q-fab-action
      @click="useServerStore().supportArchive()"
      color="secondary"
      icon="mdi-bash"
      :label="$t('general.server.supportArchive.title')"
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
    <q-fab-action
      @click="useServerStore().reboot()"
      color="secondary"
      icon="mdi-restart"
      :label="$t('general.server.reboot.title')"
      label-position="left"
      square
    />
    <q-fab-action
      @click="useServerStore().shutdown()"
      color="secondary"
      icon="mdi-stop"
      :label="$t('general.server.shutdown.title')"
      label-position="left"
      square
    />
  </q-fab>
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
import { ref, toRefs } from 'vue';
import VersionsList from 'components/layout/VersionsList.vue';
import { useVersionsStore } from 'stores/versions';
import { useServerStore } from 'stores/server';

useVersionsStore().fetch();

const props = defineProps({
  model: { type: Boolean, required: true },
});

const { model } = toRefs(props);

const open = ref<boolean>(false);
const emit = defineEmits(['toggle']);

function update(value:boolean) {
  emit('toggle', { label: 'helpButton', value });
}
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

  #help-button.q-fab--opened
    .q-focus-helper
      background: currentColor
      opacity: 0.15
</style>
