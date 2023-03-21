<template>
  <q-toggle
    :label="$t(label)"
    class="q-mb-md"
    v-model="enabled"
    v-bind="$attrs"
    color="accent"
  />
</template>

<script lang="ts" setup>
import { computed, toRefs } from 'vue';

const props = defineProps({
  label: { type: String, required: true },
  setter: { type: Function, required: true },
  getter: { type: Object, required: true },
  apiKey: { type: String, required: true },
});

const {
  label, getter, setter, apiKey,
} = toRefs(props);

const enabled = computed({
  get: () => getter.value.value,
  set: (selected) => setter.value({ [apiKey.value]: selected }),
});
</script>

<style lang="sass">

</style>
