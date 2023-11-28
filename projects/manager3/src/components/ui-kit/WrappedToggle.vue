<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div class="col col-xs-12 col-sm-12 col-md-12">
    <q-toggle
      :label="$t(label)"
      class="q-mb-md"
      v-model="enabled"
      v-bind="$attrs"
      color="accent"
      checked-icon="check"
      unchecked-icon="clear"
    />
    <HelpButton :warning="warning" toggle v-if="help">
      <template v-slot:help>
        <slot name="help"></slot>
      </template>
    </HelpButton>
  </div>
</template>

<script lang="ts" setup>
import { computed, toRefs } from 'vue';
import HelpButton from 'components/ui-kit/HelpButton.vue';

const props = defineProps({
  label: { type: String, required: true },
  help: { type: Boolean, default: false },
  warning: { type: Boolean },
  setter: { type: Function, required: true },
  getter: { type: Object, required: true },
  apiKey: { type: String, required: true },
});

const {
  label, help, warning, getter, setter, apiKey,
} = toRefs(props);

const enabled = computed({
  get: () => getter.value.value,
  set: (selected) => setter.value({ [apiKey.value]: selected }),
});
</script>
