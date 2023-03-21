<template>
  <q-page-sticky
    :offset="[18, 18]"
    position="bottom-right"
    style="transform: translate(-74px, 0px);"
  >
    <q-btn
      padding="16px"
      flat
      square
      icon="mdi-keyboard"
      color="primary"
      id="virtual-devices-button"
      @click="show(true)"
      disable
    />
  </q-page-sticky>
</template>

<script lang="ts" setup>
import { useQuasar } from 'quasar';
import { useRouter } from 'vue-router';
import { useI18n } from 'vue-i18n';

const $q = useQuasar();
const router = useRouter();
const { t } = useI18n({ useScope: 'global' });

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
