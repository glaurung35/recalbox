<template>
  <q-select
    :label="$t(label)"
    :options="options"
    class="q-mb-md"
    dense
    standout="bg-primary text-white"
    v-model="value"
    v-bind="$attrs"
    :map-options="true"
  >
    <template v-slot:after>
      <HelpButton :help="help" :warning="warning"/>
    </template>
  </q-select>
</template>

<script lang="ts" setup>
import { computed, toRefs } from 'vue';
import HelpButton from 'components/global/HelpButton.vue';

const props = defineProps({
  label: { type: String, required: true },
  help: { type: String },
  warning: { type: Boolean },
  options: { type: Object, required: true },
  setter: { type: Function, required: true },
  getter: { type: Object, required: true },
  apiKey: { type: String, required: true },
});

const {
  label, help, warning, options, getter, setter, apiKey,
} = toRefs(props);

const value = computed({
  get: () => getter?.value.value,
  set: (selected) => setter.value({ [apiKey?.value]: selected }),
});
</script>
