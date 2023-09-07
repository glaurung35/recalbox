/**
 * @author Pit64
 */
import { z } from 'zod';

export const ServerListSchema = z.object({
  linkResponse: z.string(),
});

export type ServerList = z.infer<typeof ServerListSchema>
