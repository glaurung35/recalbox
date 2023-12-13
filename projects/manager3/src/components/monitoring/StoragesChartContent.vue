<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <q-linear-progress
    v-for="item in items"
    size="60px"
    :value="item.used"
    color="accent"
    class="q-mt-sm progress"
    :key="item.mount"
    track-color="white"
    rounded
  >
    <div class="absolute-full flex items-center">
      <q-icon :name="item.icon" color="secondary"/>
      <div class="progress-text">
        {{ item.recalbox }} | {{ (item.used * 100).toFixed(2) }}%
      </div>
    </div>
  </q-linear-progress>
</template>

<script lang="ts" setup>
import { computed, toRefs } from 'vue';

const props = defineProps({
  data: { type: Object, required: true },
});

const { data } = toRefs(props);

interface Item {
  recalbox: string;
  icon: string;
  mount: string;
  used: number;
}
const items = computed(() => {
  let result: Item[] = [];
  const storages = JSON.parse(JSON.stringify(data.value));

  Object.keys(storages).forEach((key) => {
    result = {
      ...result,
      [storages[key].mount]: {
        recalbox: storages[key].recalbox,
        icon: 'mdi-harddisk',
        mount: storages[key].mount,
        used: storages[key].used / storages[key].size,
      },
    };
  });
  return result;
});
</script>

<style lang="sass">
.progress-text
  display: flex
  align-items: center
  font-size: 12px!important
  font-weight: bold
  text-transform: capitalize
</style>
