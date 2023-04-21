<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <q-select
    :label="$t(label)"
    :options="options"
    class="q-mb-md"
    standout="bg-primary text-white"
    v-model="value"
    v-bind="$attrs"
    :clearable="clearable"
    multiple
  >
    <template v-slot:selected-item />
    <template v-slot:option="{ itemProps, opt, selected, toggleOption }">
      <q-item v-bind="itemProps">
        <q-item-section>
          <q-item-label v-html="opt" />
        </q-item-section>
        <q-item-section side>
          <q-checkbox
            :model-value="selected"
            @update:model-value="toggleOption(opt)"
            color="accent"
          />
        </q-item-section>
      </q-item>
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
  clearable: { type: Boolean, default: false },
});

const {
  label, help, warning, options, getter, setter, apiKey,
} = toRefs(props);

const value = computed({
  get: () => {
    console.log(getter?.value.value);
    return getter?.value.value;
  },
  set: (selected) => setter.value({ [apiKey?.value]: selected === null ? null : selected }),
});
</script>
