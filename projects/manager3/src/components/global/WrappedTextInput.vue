<template>
  <q-form autocomplete="off">
    <q-input
      :label="$t(label)"
      dense
      standout="bg-primary text-white"
      v-model="text"
      v-bind="$attrs"
      class="q-mb-md"
      :type="isPwd && password ? 'password' : 'text'"
    >
      <template v-slot:append v-if="password">
        <q-icon
          :name="isPwd ? 'mdi-eye-off' : 'mdi-eye'"
          class="cursor-pointer"
          @click="isPwd = !isPwd"
        />
      </template>
    </q-input>
  </q-form>
</template>

<script lang="ts" setup>
import { toRefs, ref, computed } from 'vue';

const props = defineProps({
  label: { type: String, required: true },
  setter: { type: Function, required: true },
  getter: { type: Object, required: true },
  apiKey: { type: String, required: true },
  password: { type: Boolean },
});

const {
  label, getter, setter, apiKey, password,
} = toRefs(props);

const timeout = ref();
const isPwd = ref<boolean>(true);

const text = computed({
  get: () => getter.value.value,
  set: (value) => {
    clearTimeout(timeout.value);

    timeout.value = setTimeout(() => {
      setter.value({ [apiKey.value]: value });
    }, 700);
  },
});
</script>
