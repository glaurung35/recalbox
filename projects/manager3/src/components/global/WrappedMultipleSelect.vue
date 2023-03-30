<template>
  <q-select
    :label="$t(label)"
    :options="options"
    class="q-mb-md"
    dense
    standout="bg-primary text-white"
    v-model="value"
    v-bind="$attrs"
    clearable
    multiple
    use-chips
  >
    <template v-slot:selected>
      <q-chip
        square
        color="accent"
        text-color="white"
        dense
        removable
      >
        {{ value }}
      </q-chip>
    </template>
    <template v-slot:no-option>
      <q-item>
        <q-item-section>
          {{ $t('components.wrappedSelect.noResults') }}
        </q-item-section>
      </q-item>
    </template>
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
  options: { type: Object, required: true, default() { return []; } },
  setter: { type: Function, required: true },
  getter: { type: Object, required: true },
  apiKey: { type: String, required: true },
});

const {
  label, help, warning, options, getter, setter, apiKey,
} = toRefs(props);

const value = computed({
  get: () => getter?.value.value,
  set: (selected) => setter.value({ [apiKey?.value]: selected === null ? null : selected }),
});
</script>
