<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <q-page-sticky
    position="bottom"
    style="transform: translate(-38px, 0px);"
  >
    <q-btn
      padding="16px"
      flat
      square
      icon="mdi-keyboard"
      color="primary"
      id="virtual-devices-button"
      @click="show(true)"
      :disable="!system['virtual-gamepads.enabled']?.value"
    >
      <q-tooltip class="bg-primary" anchor="top middle" self="bottom middle" :offset="[10, 10]">
        {{ $t('general.virtualDevices.tooltip') }}
      </q-tooltip>
    </q-btn>
  </q-page-sticky>
</template>

<script lang="ts" setup>
import { useQuasar } from 'quasar';
import { useRouter } from 'vue-router';
import { useI18n } from 'vue-i18n';
import { useSystemStore } from 'stores/configuration/system';
import { storeToRefs } from 'pinia';

const $q = useQuasar();
const router = useRouter();
const { t } = useI18n({ useScope: 'global' });

const systemStore = useSystemStore();
systemStore.fetch();
const { system } = storeToRefs(systemStore);

function show(display:boolean) {
  $q.bottomSheet({
    message: t('general.virtualDevices.title'),
    class: 'virtual-devices-menu',
    grid: display,
    dark: true,
    actions: [
      {
        label: t('general.virtualDevices.keyboard'),
        icon: 'mdi-keyboard-outline',
        id: 'keyboard',
        route: 'virtual-devices-keyboard',
      },
      {
        label: t('general.virtualDevices.gamepad'),
        icon: 'mdi-gamepad-variant-outline',
        id: 'gamepad',
        route: 'virtual-devices-gamepad',
      },
      {
        label: t('general.virtualDevices.trackpad'),
        icon: 'mdi-trackpad',
        id: 'trackpad',
        route: 'virtual-devices-trackpad',
      },
    ],
  }).onOk((button) => {
    const routeData = router.resolve({ name: button.route });
    window.open(routeData.href, '_blank');
  });
}
</script>

<style lang="sass">
#virtual-devices-button
  border-radius: 4px

.virtual-devices-menu
  width: inherit

  .q-bottom-sheet__item
    min-width: 140px
</style>
