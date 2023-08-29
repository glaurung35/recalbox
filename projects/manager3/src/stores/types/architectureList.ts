/**
 * @author Pit64
 */
import { z } from 'zod';

export const ArchitectureListSchema = z.object({
  arch: z.string(),
});

export type ArchitectureList = z.infer<typeof ArchitectureListSchema>
