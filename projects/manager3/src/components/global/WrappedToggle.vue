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
    <HelpButton :help="help" :warning="warning" toggle/>
  </div>
</template>

<script lang="ts" setup>
import { computed, toRefs } from 'vue';
import HelpButton from 'components/global/HelpButton.vue';

const props = defineProps({
  label: { type: String, required: true },
  help: { type: String },
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
