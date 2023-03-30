<template>
  <div class="disks-usage col">
    <q-linear-progress
      v-for="item in items"
      size="30px"
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
          <span>
            {{ item.mount }} <b>| {{ (item.used * 100).toFixed(2) }}%</b>
          </span>
        </div>
      </div>
    </q-linear-progress>
  </div>
</template>

<script lang="ts" setup>
import { computed, toRefs } from 'vue';

const props = defineProps({
  data: { type: Object, required: true },
});

const { data } = toRefs(props);

const items = computed(() => {
  let result: object = {};
  const storages = JSON.parse(JSON.stringify(data.value));

  Object.keys(storages).forEach((key) => {
    result = {
      ...result,
      [storages[key].mount]: {
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
.disks-usage
  flex: 1
  padding: .5em

  .progress
    border: 1px solid rgba(240, 240, 240, 1)

    .q-linear-progress__model
      opacity: .5

    .progress-text
      height: 100%
      line-height: 28px
      font-size: .4em
      color: $primary

      span
        vertical-align: middle

    i
      font-size: .7em
      margin-left: .2em
      margin-right: .2em
</style>
