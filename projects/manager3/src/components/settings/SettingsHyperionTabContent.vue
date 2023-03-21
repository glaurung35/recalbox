<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <FormFragmentContainer title="settings.hyperion.settings.title">
        <template v-slot:content>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedToggle
              label="settings.hyperion.settings.enableHyperion"
              :getter="hyperion.enabled"
              :setter="hyperionStore.post"
              apiKey="enabled"
              v-if="hyperion.enabled"
            />
          </div>
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script lang="ts" setup>
import WrappedToggle from 'components/global/WrappedToggle.vue';
import { useHyperionStore } from 'stores/hyperion';
import { storeToRefs } from 'pinia';
import FormFragmentContainer from '../global/FormFragmentContainer.vue';

const hyperionStore = useHyperionStore();
hyperionStore.fetch();
const { hyperion } = storeToRefs(hyperionStore);
</script>
